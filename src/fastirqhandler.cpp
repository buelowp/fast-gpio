/*
 * Fast GPIO Library for the Onion.io Omega2+ board
 * Copyright (C) 2019  Peter Buelow <goballstate at gmail>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <fastirqhandler.h>

void FastIRQHandler::run()
{
    std::map<int, FastGPIO*> active;
    struct pollfd fds[18];
    int index = 0;
    int pollrc = 0;
    
    onionPrint(ONION_SEVERITY_DEBUG, "Adding %d entries to the poll function\n", m_metadata.size());
    for (std::map<int,FastGPIO*>::iterator it = m_metadata.begin(); it != m_metadata.end(); ++it) {
        fds[index].fd = it->second->fd();
		fds[index].events = POLLPRI;
        active.insert(std::pair<int, FastGPIO*>(it->second->fd(), it->second));
        index++;
        onionPrint(ONION_SEVERITY_DEBUG, "Added pollfd entry %d, fd %d\n", index, it->second->fd());
    }

    while (m_enabled) {
        if ((pollrc = poll(fds, index, 100)) < 0) {
            if (errno == EINTR)
                continue;
            else {
                onionPrint(ONION_SEVERITY_FATAL, "poll failed: %d\n", errno);
                m_enabled = false;
                return;
            }
        }
        else if (pollrc > 0) {
            for (int i = 0; i < pollrc; i++) {
                if (fds[i].revents & POLLPRI) {
                    auto it = active.find(fds[i].fd);
                    if (it != active.end()) {
                        std::function<void()> func = it->second->callback();
                        try {
                            onionPrint(ONION_SEVERITY_DEBUG, "Executing callback for pin %d\n", it->second->pin());
                            if (checkDebounce(&(*it->second)))
                                func();
                        }
                        catch (const std::bad_function_call& e) {
                            onionPrint(ONION_SEVERITY_FATAL, "Unable to execute callback for pin %d\n", it->second->pin());
                            onionPrint(ONION_SEVERITY_FATAL, "exception: %s\n", e.what());
                        }
                    }
                }
            }
        }
        else {
            continue;
        }
    }
}

bool FastIRQHandler::checkDebounce(FastGPIO *pin)
{
    timeval timeNow;
    gettimeofday(&timeNow, NULL);
    suseconds_t nowMs = (timeNow.tv_sec * 1000L) + (timeNow.tv_usec / 1000L);

    suseconds_t timeDiff = nowMs - pin->time();

    // Don't want it if insufficient time has elapsed for debounce
    if (timeDiff < pin->debounce())
        return false;

    pin->setTime(nowMs);
    return true;
}

FastGPIO* FastIRQHandler::findPin(int pin)
{
	auto it = m_metadata.find(pin);
	if (it != m_metadata.end()){
		return &(*it->second);
	}

	return nullptr;
}

int FastIRQHandler::clear(int pin)
{
	std::lock_guard<std::mutex> guard(m_mutex);

	if (m_metadata.find(pin) == m_metadata.end()) {
		onionPrint(ONION_SEVERITY_DEBUG, "Pin %d is not active\n", pin);
		return m_metadata.size();
	}

	gpioEdge(pin, GPIO_IRQ_NONE);
    unexportPin(pin);
    m_enabled = false;
    auto it = m_metadata.find(pin);
    if (it != m_metadata.end()) {
        m_metadata.erase(it);
    }
    
    return m_metadata.size();
}

bool FastIRQHandler::gpioEdge(int pin, GPIO_Irq_Type type)
{
    int fd;
    char buf[128];
    char path[256];
    
    sprintf(path, "/sys/class/gpio%d/edge", pin);
    if ((fd = open(path, O_WRONLY)) > 0) {
        switch (type) {
            case GPIO_IRQ_RISING:
                sprintf(buf, "rising");
                break;
            case GPIO_IRQ_FALLING:
                sprintf(buf, "falling");
                break;
            case GPIO_IRQ_BOTH:
                sprintf(buf, "both");
                break;
            case GPIO_IRQ_NONE:
                sprintf(buf, "none");
                break;
        }
		if (write(fd, buf, strlen(buf) + 1) < 0) {
			onionPrint(ONION_SEVERITY_FATAL, "Error writing %s to %s: %d\n", buf, path, errno);
			close(fd);
			return false;
		}
    }
    close(fd);
    return true;
}

bool FastIRQHandler::exportPin(int pin)
{
    int fd;
    char buf[128];
    
    if ((fd = open("/sys/class/gpio/export", O_WRONLY)) > 0) {
		sprintf(buf, "%d", pin);
		onionPrint(ONION_SEVERITY_DEBUG, "Writing %s to /sys/class/gpio/export\n", buf);
		if (write(fd, buf, strlen(buf) + 1) < 0) {
            if (errno == 16) {
                onionPrint(ONION_SEVERITY_DEBUG, "Pin %d has been exported, assuming control\n", pin);
            }
            else {
                onionPrint(ONION_SEVERITY_FATAL, "Error writing %s to export: %s(%d)\n", buf, strerror(errno), errno);
                close(fd);
                return false;
            }
		}
    }
    close(fd);
    return true;
}

bool FastIRQHandler::unexportPin(int pin)
{
    int fd;
    char buf[128];
    
    auto it = m_metadata.find(pin);
    if (it != m_metadata.end()) {
        close(it->second->fd());
    }
    gpioEdge(pin, GPIO_IRQ_NONE);
    if ((fd = open("/sys/class/gpio/unexport", O_WRONLY)) > 0) {
		sprintf(buf, "%d", pin);
		onionPrint(ONION_SEVERITY_DEBUG, "Writing %s to /sys/class/gpio/unexport\n", buf);
		if (write(fd, buf, strlen(buf) + 1) < 0) {
			onionPrint(ONION_SEVERITY_FATAL, "Error writing pin to export\n", errno);
			close(fd);
			return false;
		}
    }
    close(fd);
    return true;
}

int FastIRQHandler::gpioValue(int pin)
{
    char path[256];
    
    sprintf(path, "/sys/class/gpio/gpio%d/value", pin);
    onionPrint(ONION_SEVERITY_DEBUG, "Opening %s\n", path);
    return open(path, O_RDONLY | O_NONBLOCK);
}

bool FastIRQHandler::set(FastGPIO *pin)
{
	int fd;

	std::lock_guard<std::mutex> guard(m_mutex);

	if (pin->direction() != GPIO_DIRECTION_IN) {
		onionPrint(ONION_SEVERITY_DEBUG, "Pin is set as output, cannot continue\n");
		return false;
	}

	if (m_metadata.find(pin->pin()) != m_metadata.end()) {
		onionPrint(ONION_SEVERITY_DEBUG, "Pin %d is already active, cancel first\n", pin);
		return false;
	}

	if (!exportPin(pin->pin()))
        return false;
    
    if (!gpioEdge(pin->pin(), pin->type())) {
        unexportPin(pin->pin());
        return false;
    }
    
   if ((fd = gpioValue(pin->pin())) < 0) {
        onionPrint(ONION_SEVERITY_FATAL, "Unable to open gpio value file: %s(%d)\n", strerror(errno), errno);
        unexportPin(pin->pin());
        return false;
    }
    pin->setFd(fd);
    m_metadata.insert(std::pair<int, FastGPIO*>(pin->pin(), pin));
    
	return true;
}

void FastIRQHandler::start()
{
    std::thread(run);
    m_enabled = true;
    onionPrint(ONION_SEVERITY_DEBUG, "Enabling IRQ Handler\n");
}

void FastIRQHandler::stop()
{
    m_enabled = false;
    onionPrint(ONION_SEVERITY_DEBUG, "Disabling IRQ Handler\n");
}

