# Porcupine SDK for Arduino boards - English language

Made in Vancouver, Canada by [Picovoice](https://picovoice.ai)

Porcupine is a highly-accurate and lightweight wake word engine. It enables building always-listening voice-enabled
applications. It is

- using deep neural networks trained in real-world environments.
- compact and computationally-efficient. It is perfect for IoT.
- scalable. It can detect multiple always-listening voice commands with no added runtime footprint.
- self-service. Developers can train custom wake word models using [Picovoice Console](https://picovoice.ai/console/).

## Compatibility

- [Arduino Nano 33 BLE Sense](https://docs.arduino.cc/hardware/nano-33-ble)

## Dependency

- LibPrintf

## AccessKey

The Porcupine SDK requires a valid `AccessKey` at initialization. `AccessKey`s act as your credentials when using Porcupine SDKs.
You can create your `AccessKey` for free. Make sure to keep your `AccessKey` secret.

To obtain your `AccessKey`:

1. Login or Signup for a free account on the [Picovoice Console](https://picovoice.ai/console/).
2. Once logged in, go to the [`AccessKey` tab](https://console.picovoice.ai/access_key) to create one or use an existing `AccessKey`.

## Integration

1. define all the necessary variables before `setup()`:

```c
#include <Porcupine_EN.h>

#define MEMORY_BUFFER_SIZE ...
static uint8_t memory_buffer[MEMORY_BUFFER_SIZE] __attribute__((aligned(16));

static const char* ACCESS_KEY = ...; //AccessKey string obtained from [Picovoice Console](https://picovoice.ai/console/)

const uint8_t keyword_array[] = {...};
const int32_t keyword_model_sizes = sizeof(keyword_array);
const void *keyword_models = keyword_array;
static const float SENSITIVITY = 0.75f;

pv_porcupine_t *handle = NULL;
```

Sensitivity is the parameter that enables developers to trade miss rate for false alarm. It is a floating-point number
within [0, 1]. A higher sensitivity reduces miss rate (false reject rate) at cost of increased false alarm rate.

`handle` is an instance of Porcupine runtime engine that detects utterances of wake phrase defined in `keyword_array`.

2. put the following code block inside `setup()` in order to initialize the Picovoice engine:

```c
const pv_status_t status = pv_porcupine_init(
        ACCESS_KEY,
        MEMORY_BUFFER_SIZE,
        memory_buffer,
        1,
        &keyword_model_sizes,
        &keyword_models,
        &SENSITIVITY,
        &handle);

if (status != PV_STATUS_SUCCESS) {
    // error handling logic
}
```

Picovoice accepts single channel, 16-bit PCM audio. The sample rate can be retrieved using `pv_sample_rate()`. Picovoice accepts input audio in consecutive chunks (aka frames); the length of each frame can be retrieved using `pv_porcupine_frame_length()`. Inside the `loop()` function in the sketch, pass the recorded audio to the Picovoice engine:

```cpp
const int16_t *pcm = picovoice::porcupine::pv_audio_rec_get_new_buffer()
int32_t keyword_index;
const pv_status_t status = pv_porcupine_process(handle, pcm, &keyword_index);
if (status != PV_STATUS_SUCCESS) {
    // error handling logic
}
if (keyword_index != -1) {
    // detection event logic/callback
}
```

## Create Custom Wake Word

1. Compile and upload the `Porcupine_EN/GetUUID` sketch from the `File -> Examples` menu. Copy the UUID of the board printed at the beginning of the session to the serial monitor.
2. Go to [Picovoice Console](https://console.picovoice.ai/) to create models for [Porcupine wake word engine](https://picovoice.ai/docs/quick-start/console-porcupine/).
3. Select `Arm Cortex M` as the platform when training the model.
4. Select your board type (`Arduino Nano 33 BLE Sense`) and provide the UUID of the chipset on the board.

The model is now being trained. You will be able to download it within a few hours.

## Import the Custom Wake Word

1. Download your custom voice model(s) from [Picovoice Console](https://console.picovoice.ai/).
2. Decompress the zip file. The model for Porcupine wake word is located in two files: A binary `.ppn` file, and as a `.h` header file containing a `C` array version of the binary model.
3. Copy the contents of the array inside the `.h` header file and update update the `DEFAULT_KEYWORD_ARRAY` values in `params.h`.
