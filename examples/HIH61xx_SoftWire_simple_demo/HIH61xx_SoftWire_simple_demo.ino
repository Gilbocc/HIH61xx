// This example demonstrates how to use the HIH61xx class with the SoftWire library. SoftWire is a software I2C
// implementation which enables any two unused pins to be used as a I2C bus. A blocking read is made to the
// HIH61xx device. See HIH61xx_SoftWire_demo for a more sophisticated example which allows other tasks to run
// whilst the HIH61xx takes its measurements.

#include <SoftWire.h>
#include <HIH61xx.h>
#include <AsyncDelay.h>

// Create an instance of the SoftWire class called "sw". In this example it uses the same pins as the hardware I2C
// bus. Pass the pin numbers to use different pins.
SoftWire sw(SDA, SCL);

// The "hih" object must be created with a reference to the SoftWire "sw" object which represents the I2C bus it is
// using. Note that the class for the SoftWire object must be included in the templated class name.
HIH61xx<SoftWire> hih(sw);

AsyncDelay samplingInterval;

// SoftWire requires that the programmer declares the buffers used. This allows the amount of memory used to be set
// according to need. For the HIH61xx only a very small RX buffer is needed.
uint8_t i2cRxBuffer[4];
uint8_t i2cTxBuffer[32];

void setup(void)
{
#if F_CPU >= 12000000UL
    Serial.begin(115200);
#else
	Serial.begin(9600);
#endif

    // The pin numbers for SDA/SCL can be overridden at runtime.
	// sw.setSda(sdaPin);
	// sw.setScl(sclPin);


	sw.setRxBuffer(i2cRxBuffer, sizeof(i2cRxBuffer));
	//sw.setTxBuffer(i2cTxBuffer, sizeof(i2cTxBuffer));

	// HIH61xx doesn't need a TX buffer at all but other I2C devices probably will.
	//sw.setTxBuffer(i2cTxBuffer, sizeof(i2cTxBuffer));
	sw.setTxBuffer(NULL, 0);

	sw.begin();  // Sets up pin mode for SDA and SCL

	hih.initialise();
	samplingInterval.start(3000, AsyncDelay::MILLIS);
}


void loop(void)
{
    // Instruct the HIH61xx to take a measurement. This blocks until the measurement is ready.
    hih.read();

    // Fetch and print the results
    Serial.print("Relative humidity: ");
    Serial.print(hih.getRelHumidity() / 100.0);
    Serial.println(" %");
    Serial.print("Ambient temperature: ");
    Serial.print(hih.getAmbientTemp() / 100.0);
    Serial.println(" deg C");
    Serial.print("Status: ");
    Serial.println(hih.getStatus());

    // Wait a second
    delay(1000);
}