# fast-gpio
Based on the original Onion IoT fast-gpio binary implementation.

This is the library version of that, stripped down a bit.

# Using it with C++
Build it, install to the toolchain path.

You link against -loniongpio

The easiest usage is the following for a simple GPIO set/get

```
#include <iostream>
#include <fastgpioomega2.h>

int main() {
	FastGpioOmega2 omega2;

	omega2.SetDirection(3, 1);
	omega2.Set(3, 1);

	std::cout << "Turned GPIO on" << std::endl;
	return 0;
}
```

More to come
