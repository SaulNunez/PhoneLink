# PhoneLink

PhoneLink is a project for repurposing old landline phones as bluetooth handsets for you smartphone.

## Uses
While the idea for this project is for reviving an old dial phone as a way to receive and place calls to my smartphone, this project is excellent for seniors who don't like to use their smartphones and instead would like a experience similar to phone lines, just pair the module via bluetooth to their smartphone and connect the module to their landline phone(s).

## Hardware
* ESP32
* MAX98357: As an external DAC that accepts I2S from the microcontroller.
* PCM1808: As an external ADC that generates a I2S bus that can be used by the microcontroller.
* KS0835F: This external module is in charge of creating the voltages for connecting to a landline phone, as well as provide information via pin voltage changes when the phone is unhooked.
* MT8870: Chip that reads the audio input and provides via decimal coded binary, which number is pressed (for more information check out [DTMF](https://en.wikipedia.org/wiki/DTMF_signaling)).
