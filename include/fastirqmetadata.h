#ifndef __FASTGPIOIRQMETADATA_H__
#define __FASTGPIOIRQMETADATA_H__

#include <functional>

#include <signal.h>
#include <sys/time.h>

#include "fastgpiotypes.h"

class FastIRQMetaData {
public:
	FastIRQMetaData()
	{
		m_type = GPIO_IRQ_NONE;
		m_debounce = 0;
		m_lastMillis = 0;
	}
	FastIRQMetaData(GPIO_Irq_Type t, unsigned long d, std::function<void()> cbk)
	{
		setType(t);
		setDebounce(d);
		setCallback(cbk);
		m_lastMillis = 0;
	}
	~FastIRQMetaData();

	std::function<void()> callback() { return m_callback; }
	void setCallback(std::function<void()> cbk) { m_callback = cbk; }

	GPIO_Irq_Type type() { return m_type; }
	void setType(GPIO_Irq_Type t) { m_type = t; }

	unsigned long debounce() { return m_debounce; }
	void setDebounce(unsigned long d) { m_debounce = d; }

	suseconds_t time() { return m_lastMillis; }
	void setTime(suseconds_t m) { m_lastMillis = m; }

private:
	std::function<void()> m_callback;
	GPIO_Irq_Type m_type;
	unsigned long m_debounce;
	suseconds_t m_lastMillis;
};

#endif
