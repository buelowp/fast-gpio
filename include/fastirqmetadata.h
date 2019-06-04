#ifndef __FASTGPIOIRQMETADATA_H__
#define __FASTGPIOIRQMETADATA_H__

#include <functional>

#include <fastgpiotypes.>

class FastIRQMetaData {
public:
	FastIRQMetaData()
	{
		m_type = GPIO_IRQ_NONE;
		m_debounce = 0;
	}
	FastIRQMetaData(int t, unsigned long d, std::function<void()> cbk)
	{
		setType(t);
		setDebounce(d);
		setCallback(cbk);
	}
	~FastIRQMetaData();

	std::function<void()> callback() { return m_callback; }
	void setCallback(std::function<void()> cbk) { m_callback = cbk; }

	GPIO_Irq_Type type() { return m_type; }
	void setType(GPIO_Irq_Type t) { m_type = t; }

	unsigned long debounce() { return m_debounc; }
	void setDebounce(unsigned long d) { m_debounce = d; }

private:
	std::function<void()> m_callback;
	GPIO_Irq_Type m_type;
	unsigned long m_debounce;
};
#endif
