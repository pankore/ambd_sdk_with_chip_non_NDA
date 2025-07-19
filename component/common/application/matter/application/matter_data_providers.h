/*
 *    This module is a confidential and proprietary property of RealTek and
 *    possession or use of this module requires written permission of RealTek.
 *
 *    Copyright(c) 2025, Realtek Semiconductor Corporation. All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/******************************************************
 *           Matter Data Providers Declaration
 ******************************************************/

#define AMEBA_DATA_PROVIDER_TEST    1

#define MAX_FIXED_LABELS_COUNT      4
#define MAX_CALENDAR_TYPE_COUNT     12
#define MAX_ACTIVE_LOCALE_LENGTH    35
#define MAX_LABEL_NAME_LENGTH       16
#define MAX_LABEL_VALUE_LENGTH      16

/******************************************************
 *           Matter Data Providers Structure
 ******************************************************/

typedef struct {
    char key[MAX_LABEL_NAME_LENGTH];
    char value[MAX_LABEL_VALUE_LENGTH];
    bool isSet;
} ameba_fixed_label_t;

/******************************************************
 *           Matter Data Providers Function
 ******************************************************/

#ifdef __cplusplus
extern "C" {
#endif

bool matter_set_fixed_label(uint8_t index, const char * key, const char * value);
size_t matter_get_fixed_label_count(void);
const char * matter_get_fixed_label_name(uint8_t index);
const char * matter_get_fixed_label_value(uint8_t index);
size_t matter_get_supported_locale_count(void);
const char * matter_get_supported_locale_value(uint8_t index);
size_t matter_get_calendar_type_count(void);
bool matter_get_calendar_type_value(uint8_t index, uint8_t *output);

#if AMEBA_DATA_PROVIDER_TEST
void test_example_set_fixed_label(void);
void test_example_set_supported_locale(void);
void test_example_set_calendar_type(void);
#endif // AMEBA_DATA_PROVIDER_TEST

#ifdef __cplusplus
}
#endif
