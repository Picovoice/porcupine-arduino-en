/*
    Copyright 2021 Picovoice Inc.

    You may not use this file except in compliance with the license. A copy of the license is located in the "LICENSE"
    file accompanying this source.

    Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
    an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
    specific language governing permissions and limitations under the License.
*/

#include <Porcupine_EN.h>

#include "params.h"

#define MEMORY_BUFFER_SIZE (70 * 1024)

static const char* ACCESS_KEY = ... //AccessKey string obtained from Picovoice Console (https://picovoice.ai/console/)

static int8_t memory_buffer[MEMORY_BUFFER_SIZE] __attribute__((aligned(16)));

static pv_porcupine_t *handle = NULL;

static const int32_t KEYWORD_MODEL_SIZES = sizeof(KEYWORD_ARRAY);
static const void *KEYWORD_MODELS = KEYWORD_ARRAY;
static const float SENSITIVITY = 0.75f;

static void wake_word_callback(void) {
    Serial.println("Wake word detected!");
}

void setup() {

    Serial.begin(9600);
    while (!Serial);

    pv_status_t status = pv_audio_rec_init();
    if (status != PV_STATUS_SUCCESS) {
        Serial.print("Audio init failed with ");
        Serial.println(pv_status_to_string(status));
        while (1);
    }

    status = pv_porcupine_init(
            ACCESS_KEY,
            MEMORY_BUFFER_SIZE,
            memory_buffer,
            1,
            &KEYWORD_MODEL_SIZES,
            &KEYWORD_MODELS,
            &SENSITIVITY,
            &handle);
    if (status != PV_STATUS_SUCCESS) {
        Serial.print("Picovoice init failed with ");
        Serial.println(pv_status_to_string(status));
        while (1);
    }
    Serial.println("The board is listening for 'Porcupine'...");
}

void loop()
{
    const int16_t *buffer = pv_audio_rec_get_new_buffer();
    if (buffer) {
        int32_t keyword_index;
        const pv_status_t status = pv_porcupine_process(handle, buffer, &keyword_index);
        if (status != PV_STATUS_SUCCESS) {
            Serial.print("Picovoice process failed with ");
            Serial.println(pv_status_to_string(status));
            while(1);
        }
        if (keyword_index != -1) {
            wake_word_callback();
        }
    }
}
