/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2016 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/
 
/**
 * @file dsAudio.h
 *
 * @brief Device Settings HAL Audio Public APIs.
 * These APIs define the HAL for the Device Settings Audio interface.
 *
 * @par Document
 * Document reference.
 *
 * @par Open Issues (in no particular order)
 * -# None
 *
 * @par Assumptions
 * -# None
 *
 * @par Abbreviations
 * - `DS`     - Device Settings
 * - `HAL`    - Hardware Abstraction Layer
 * - `API`    - Application Programming Interface
 * - `Caller` - Any user of the interface via the `APIs`
 * - `CB`     - Callback function (suffix)
 * - `ARC`    - Audio Return Channel
 * - `eARC`   - Enhanced Audio Return Channel
 * - `HDMI`   - High-Definition Multimedia Interface
 * - `LE`     - Loudness Equivalence
 * - `DRC`    - Dynamic Range Control
 * - `MI`     - Media Intelligent
 * - `RF`     - Radio Frequency
 * - `dB`     - Decibel
 * - `MS12`   - MultiStream 12
 * - `AC4`    - Audio Compression 4
 * - `ms`     - milliseconds
 * - `CPU`    - Central Processing Unit
 * - `SAD`    - Short Audio Descriptor
 * - `DAPV2`  - Dolby Audio Processing Version 2
 * - `DE`     - Dialog Enhacement
 *
 * @par Implementation Notes
 * -# None
 *
 */
 
/**
 * @addtogroup HPK Hardware Porting Kit
 * @{
 * @par The Hardware Porting Kit
 * HPK is the next evolution of the well-defined Hardware Abstraction Layer
 * (HAL), but augmented with more comprehensive documentation and test suites
 * that OEM or SOC vendors can use to self-certify their ports before taking
 * them to RDKM for validation or to an operator for final integration and
 * deployment. The Hardware Porting Kit effectively enables an OEM and/or SOC
 * vendor to self-certify their own Video Accelerator devices, with minimal RDKM
 * assistance.
 *
 */

/**
 * @addtogroup Device_Settings Device Settings Module
 * @{
 */

/**
 * @addtogroup Device_Settings_HAL Device Settings HAL
 * @par Application API Specification
 * Described herein are the DeviceSettings HAL types and functions that are part of
 * the Audio subsystem. The Audio subsystem manages system-specific HAL operations.
 *  @{
 */

/**
*   @defgroup dsAudio_HAL DS Audio HAL
 *  @{
 * @par Application API Specification
 * dsAudio HAL provides an interface for managing the Audio settings for the device settings module
 */

/**
 * @defgroup DSHAL_AUDIO_API DS HAL Audio Public APIs
 *  @{
 */

#ifndef _DS_AUDIOOUTPORT_H_
#define _DS_AUDIOOUTPORT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include "dsError.h"
#include "dsTypes.h"

/**
 * @brief Callback function used to notify the Audio port connection status change to the `caller`.
 *
 * HAL Implementation should call this method to deliver updated audio port connection event
 * to the `caller`.
 * 
 * @param[in] portType  - Type of the audio port where connection status is changed. Please refer ::dsAudioPortType_t
 * @param[in] uiPortNo  - Port number in which the connection status changed. @link dsAudioPORT_NUM_MAX @endlink
 * @param[in] isPortCon - Current connection status of the audio port
 *
 * @pre - dsAudioOutRegisterConnectCB
 */
typedef void (*dsAudioOutPortConnectCB_t)(dsAudioPortType_t portType, unsigned int uiPortNo, bool isPortCon);

/**
 * @brief Callback function used to notify Audio Format change to the `caller`.
 *
 * HAL Implementation should call this method to deliver updated audio format event
 * to the `caller`.
 * 
 * @param[in] audioFormat : New audio format of the active port. Please refer ::dsAudioFormat_t
 *
 * @pre - dsAudioFormatUpdateRegisterCB
 */
typedef void (*dsAudioFormatUpdateCB_t)(dsAudioFormat_t audioFormat);

/**
 * @brief Call back function used to notify audio sink Atmos capability change
 *
 * HAL Implementation should call this method to deliver updated atmos capability change event
 * to the `caller`.
 *
 * @param[in] atmosCaps  - current atmos capability of the sink device
 * @param[in] status     - atmos caps status ( @a true to enable, @a false to disable)
 *
 * @pre - dsAudioAtmosCapsChangeRegisterCB()
 */
typedef void (*dsAtmosCapsChangeCB_t) (dsATMOSCapability_t atmosCaps, bool status);

/**
 * @brief Initializes the audio port sub-system of Device Settings HAL.
 * 
 * This function initializes all the audio output ports and allocates required resources. 
 * It must return dsERR_OPERATION_NOT_SUPPORTED when there are no audio ports present in the device 
 * (e.g. a headless gateway device).
 *
 * @return dsError_t                      -  Status
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsAudioPortTerm()
 * 
 */
dsError_t  dsAudioPortInit();

/**
 * @brief Gets the audio port handle.
 * 
 * This function returns the handle for the type of audio port requested. It must return
 * dsERR_OPERATION_NOT_SUPPORTED if an unavailable audio port is requested.
 *
 * @param[in] type     - Type of audio port (HDMI, SPDIF and so on). Please refer ::dsAudioPortType_t
 * @param[in] index    - Index of audio port depending on the available ports(0, 1, ...). Maximum value of number of ports is platform specific. Please refer ::dsAudioPortConfig_t
 * @param[out] handle  - Pointer to hold the handle of the audio port
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid(port is not present or index is out of range)
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t  dsGetAudioPort(dsAudioPortType_t type, int index, intptr_t *handle);

/**
 * @brief Gets the encoding type of an audio port
 *
 * This function returns the current audio encoding setting for the specified audio port.
 *
 * @param[in] handle     -  Handle for the output audio port
 * @param[out] encoding  -  Pointer to hold the encoding setting of the audio port. Please refer ::dsAudioEncoding_t
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called in this order before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetAudioEncoding()
 */
dsError_t  dsGetAudioEncoding(intptr_t handle, dsAudioEncoding_t *encoding);

/**
 * @brief Gets the current audio format.
 *
 * This function returns the current audio format of the specified audio output port(like PCM, DOLBY AC3). Please refer ::dsAudioFormat_t
 *
 * @param[in] handle         - Handle for the output audio port
 * @param[out] audioFormat   - Pointer to hold the audio format
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t  dsGetAudioFormat(intptr_t handle, dsAudioFormat_t *audioFormat);

/**
 * @brief Gets the audio compression of the specified audio port.
 *
 * This function returns the audio compression level used in the audio port corresponding to specified port handle.
 *
 * @param[in] handle       - Handle for the output audio port
 * @param[out] compression - Pointer to hold the compression value of the specified audio port. (Value ranges from 0 to 10)
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetAudioCompression()
 */
dsError_t  dsGetAudioCompression(intptr_t handle, int *compression);

/**
 * @brief Gets the Dialog Enhancement level of the audio port.
 *
 * This function returns the dialog enhancement level of the audio port corresponding to the specified port handle.
 *
 * @param[in] handle - Handle for the output audio port
 * @param[out] level - Pointer to Dialog Enhancement level (Value ranges from 0 to 16)
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetDialogEnhancement()
 */
dsError_t  dsGetDialogEnhancement(intptr_t handle, int *level);

/**
 * @brief Gets the dolby audio mode status of an audio port.
 *
 * This function returns the dolby audio mode status used in the audio port corresponding to the specified port handle.
 *
 * @param[in] handle  - Handle for the output audio port
 * @param[out] mode   - Dolby volume mode 
 *                        ( @a true if Dolby Volume mode is enabled, and @a false if disabled)
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetDolbyVolumeMode()
 */
dsError_t  dsGetDolbyVolumeMode(intptr_t handle, bool *mode);

/**
 * @brief Gets the Intelligent Equalizer Mode.
 *
 * This function returns the Intelligent Equalizer Mode setting used in the audio port corresponding to specified Port handle.
 *
 * @param[in] handle - Handle for the output audio port.
 * @param[out] mode  - Pointer to Intelligent Equalizer mode. 0 = OFF, 1 = Open, 2 = Rich, 3 = Focused,
 *                       4 = Balanced, 5 = Warm, 6 = Detailed
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetIntelligentEqualizerMode()
 */
dsError_t  dsGetIntelligentEqualizerMode(intptr_t handle, int *mode);

/**
 * @brief Gets the Dolby volume leveller settings.
 *
 * This function returns the Volume leveller(mode and level) value used in the audio port corresponding to specified port handle.
 *
 * @param[in] handle       - Handle for the output Audio port
 * @param[out] volLeveller - Pointer to Volume Leveller. Please refer ::dsVolumeLeveller_t
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetVolumeLeveller()
 */
dsError_t  dsGetVolumeLeveller(intptr_t handle, dsVolumeLeveller_t* volLeveller);

/**
 * @brief Gets the audio Bass
 *
 * This function returns the Bass used in a given audio port
 *
 * @param[in] handle  - Handle for the output Audio port
 * @param[out] boost  - Pointer to Bass Enhancer boost value (ranging from 0 to 100)
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetBassEnhancer()
 */
dsError_t  dsGetBassEnhancer(intptr_t handle, int *boost);

/**
 * @brief Gets the audio Surround Decoder enabled/disabled status
 *
 * This function returns enable/disable status of surround decoder
 *
 * @param[in] handle   - Handle for the output Audio port
 * @param[out] enabled - Pointer to Surround Decoder enabled(1)/disabled(0) value
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsEnableSurroundDecoder()
 */
dsError_t  dsIsSurroundDecoderEnabled(intptr_t handle, bool *enabled);

/**
 * @brief Gets the DRC Mode of the specified Audio Port.
 *
 * This function returns the Dynamic Range Control used in the audio port corresponding to specified port handle.
 *
 * @param[in] handle - Handle for the output Audio port
 * @param[out] mode  - Pointer to DRC mode (0 for DRC line mode and 1 for DRC RF mode)
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetDRCMode()
 */
dsError_t  dsGetDRCMode(intptr_t handle, int *mode);

/**
 * @brief Gets the audio Surround Virtualizer level.
 *
 * This function returns the Surround Virtualizer level(mode and boost) used in the audio port corresponding to specified port handle.
 *
 * @param[in] handle       - Handle for the output Audio port
 * @param[out] virtualizer - Surround virtualizer setting. Please refer ::dsSurroundVirtualizer_t
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetSurroundVirtualizer()
 */
dsError_t  dsGetSurroundVirtualizer(intptr_t handle, dsSurroundVirtualizer_t *virtualizer);

/**
 * @brief Gets the Media Intelligent Steering of the audio port.
 *
 * This function returns enable/disable status of Media Intelligent Steering for the audio port corresponding to specified port handle.
 *
 * @param[in] handle    - Handle for the output Audio port
 * @param[out] enabled  - MI Steering enabled(1)/disabled(0) value
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetMISteering()
 */
dsError_t  dsGetMISteering(intptr_t handle, bool *enabled);

/**
 * @brief Gets the Graphic Equalizer Mode.
 *
 * This function returns the Graphic Equalizer Mode setting used in the audio port corresponding to the specified port handle.
 *
 * @param[in] handle - Handle for the output audio port.
 * @param[out] mode  - Graphic Equalizer Mode. 0 = EQ OFF, 1 = EQ Open, 2 = EQ Rich and 3 = EQ Focused 
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetGraphicEqualizerMode()
 */
dsError_t  dsGetGraphicEqualizerMode(intptr_t handle, int *mode);

/**
 * @brief Gets the supported MS12 audio profiles
 *
 * This function will get the list of supported MS12 audio profiles
 *
 * @param[in] handle     - Handle for the output Audio port
 * @param[out] profiles  - List of supported audio profiles. Please refer ::dsMS12AudioProfileList_t
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetMS12AudioProfile()
 */
dsError_t  dsGetMS12AudioProfileList(intptr_t handle, dsMS12AudioProfileList_t* profiles);

/**
 * @brief Gets current audio profile selection
 *
 * This function gets the current audio profile configured
 *
 * @param[in] handle    - Handle for the output Audio port
 * @param[out] profile  - Audio profile configured currently
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetMS12AudioProfile()
 */
dsError_t  dsGetMS12AudioProfile(intptr_t handle, char *profile);

/**
 * @brief Gets the supported ARC types of the connected ARC/eARC device
 *
 * This function gets the supported ARC types of the connected device on ARC/eARC port.
 *
 * @param[in] handle - Handle for the HDMI ARC/eARC port
 * @param[out] types - Value of supported ARC types. Please refer ::dsAudioARCTypes_t
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsGetSupportedARCTypes(intptr_t handle, int *types);

/**
 * @brief Sets Short Audio Descriptor retrieved from CEC for the connected ARC device
 *
 * This function sets the Short Audio Descriptor based on best available options
 * of Audio capabilities supported by connected ARC device. Required when ARC output
 * mode is Auto/Passthrough. Please refer ::dsAudioSADList_t, ::dsSetStereoMode
 * 
 * @param[in] handle   - Handle for the HDMI ARC/eARC port.
 * @param[in] sad_list - All SADs retrieved from CEC for the connected ARC device.
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsAudioSetSAD(intptr_t handle, dsAudioSADList_t sad_list);

/**
 * @brief Enable/Disable ARC/EARC and route audio to connected device.
 *
 * This function enables/disables ARC/EARC and routes audio to connected device. Please refer ::dsAudioARCStatus_t and ::dsAudioARCTypes_t
 *
 * @param[in] handle    - Handle for the HDMI ARC/eARC port
 * @param[in] arcStatus - ARC/eARC feature. Please refer ::dsAudioARCStatus_t
 *                          ( @a true to enable ARC/eARC, @a false to disable )
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsAudioEnableARC(intptr_t handle, dsAudioARCStatus_t arcStatus);

/**
 * @brief Gets the stereo mode of an audio port.
 * 
 * This function returns the stereo mode setting for the audio port corresponding to specified port handle.
 *
 * @param[in] handle      - Handle for the output audio port
 * @param[out] stereoMode - Pointer to hold the stereo mode setting of the
 *                            specified audio port. Please refer ::dsAudioStereoMode_t
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetStereoMode()
 */
dsError_t  dsGetStereoMode(intptr_t handle, dsAudioStereoMode_t *stereoMode);

/**
 * @brief Checks if auto mode is enabled or not for the current audio port.
 * 
 * This function returns the current auto mode of audio port corresponding to specified port handle.
 *
 * @param[in] handle     - Handle for the output audio port
 * @param[out] autoMode  - Pointer to hold the auto mode setting ( @a if enabled, @a false if disabled) of the specified audio port
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetStereoAuto()
 */	
dsError_t  dsGetStereoAuto(intptr_t handle, int *autoMode);

/**
 * @brief Gets the audio gain of an audio port.
 * 
 * This function returns the current audio gain for the audio port corresponding to specified port handle.
 *
 * @param[in] handle  - Handle for the output audio port
 * @param[out] gain   - Pointer to hold the audio gain value of the specified audio port.
                          The gain ranges between -2080 and 480
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetAudioGain()
 */
dsError_t  dsGetAudioGain(intptr_t handle, float *gain);

/**
 * @brief Gets the current audio dB level of an audio port.
 * 
 * This function returns the current audio dB level for the audio port corresponding to specified port handle.
 * The Audio dB level ranges from -1450 to 180 dB 
 * 
 * @param[in] handle  - Handle for the output audio port
 * @param[out] db     - Pointer to hold the Audio dB level of the specified audio port
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetAudioDB()
 */
dsError_t  dsGetAudioDB(intptr_t handle, float *db);

/**
 * @brief Gets the current audio volume level of an audio port.
 * 
 * This function returns the current audio volume level of audio port corresponding to specified port handle.
 *
 * @param[in] handle - Handle for the output audio port
 * @param[out] level - Pointer to hold the audio level value (ranging from 0 to 100) of the specified audio port
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetAudioLevel()
 */
dsError_t  dsGetAudioLevel(intptr_t handle, float *level);

/**
 * @brief Gets the maximum audio dB level of an audio port.
 * 
 * This function returns the maximum audio dB level supported by the audio port corresponding to specified port handle(platform specific).
 *
 * @param[in] handle  - Handle for the output audio port
 * @param[out] maxDb  - Pointer to hold the maximum audio dB value (float value e.g:10.0) supported by the specified audio port(platform specific)
 *                        Maximum value can be 180 dB
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t  dsGetAudioMaxDB(intptr_t handle, float *maxDb);

/**
 * @brief Gets the minimum audio dB level of an audio port.
 * 
 * This function returns the minimum audio dB level supported by the audio port corresponding to specified port handle.
 *
 * @param[in] handle  - Handle for the output audio port
 * @param[out] minDb  - Pointer to hold the minimum audio dB value (float. e.g: 0.0) supported by the specified audio port(platform specific)
 *                        Minimum value can be -1450 dB
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t  dsGetAudioMinDB(intptr_t handle, float *minDb);

/**
 * @brief Gets the optimal audio level of an audio port.
 * 
 * This function returns the optimal audio level (dB) of the audio port corresponding to specified port handle(platform specific).
 *
 * @param[in] handle        - Handle for the output audio port
 * @param[out] optimalLevel - Pointer to hold the optimal level value of the specified audio port(platform specific)
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t  dsGetAudioOptimalLevel(intptr_t handle, float *optimalLevel);

/**
 * @brief Gets the audio delay (in ms) of an audio port
 *
 * This function returns the audio delay (in milliseconds) of audio port with respect to video corresponding to the specified port handle.
 *
 * @param[in] handle        - Handle for the output Audio port
 * @param[out] audioDelayMs - Pointer to Audio delay ( ranges from 0 to 200 milliseconds )
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetAudioDelay()
 */
dsError_t dsGetAudioDelay(intptr_t handle, uint32_t *audioDelayMs);

/**
 * @brief Gets the audio delay offset (in ms) of an audio port.
 *
 * This function returns the audio delay offset (in milliseconds) of the audio port corresponding to specified port handle.
 *
 * @param[in] handle               - Handle for the output Audio port
 * @param[out] audioDelayOffsetMs  - Audio delay offset in milliseconds
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetAudioDelayOffset()
 */
dsError_t dsGetAudioDelayOffset(intptr_t handle, uint32_t *audioDelayOffsetMs);

/**
 * @brief Sets the audio ATMOS output mode.
 *
 * This function will set the Audio Atmos output mode.
 *
 * @param[in] handle  - Handle for the output Audio port
 * @param[in] enable  - Audio ATMOS output mode( @a true to enable  @a false to disable)
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsSetAudioAtmosOutputMode(intptr_t handle, bool enable);
  
/**
 * @brief Gets the ATMOS capability of the sink device.
 *
 * This function returns the ATMOS capability of the sink device.
 *
 * @param[in] handle       - Handle for the output Audio port
 * @param[out] capability  - ATMOS capability of sink device. Please refer ::dsATMOSCapability_t
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsGetSinkDeviceAtmosCapability(intptr_t handle, dsATMOSCapability_t *capability);

/**
 * @brief Gets the loop-through mode of an audio port.
 * 
 * This function is used to check if the audio port is configured for loop-through.
 *
 * @param[in] handle     - Handle for the output audio port
 * @param[out] loopThru  - Status of loop-through feature for the specified audio port
 *                           ( @a true when output is looped through, @a false otherwise)
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t  dsIsAudioLoopThru(intptr_t handle, bool *loopThru);

/**
 * @brief Gets the audio mute status of an audio port corresponding to the specified port handle.
 * 
 * This function is used to check whether the audio on a specified port is muted or not.
 *
 * @param[in] handle  - Handle for the output audio port
 * @param[out] muted  - Mute status of the specified audio port
 *                        ( @a true if audio is muted, @a false otherwise)
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetAudioMute()
 */
dsError_t  dsIsAudioMute(intptr_t handle, bool *muted);

/**
 * @brief Indicates whether the specified Audio port is enabled or not.
 *
 * @param[in] handle    - Handle of the output audio port
 * @param[out] enabled  - Audio port enabled/disabled state
 *                          ( @a true when audio port is enabled, @a false otherwise)
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsEnableAudioPort()
 */
dsError_t  dsIsAudioPortEnabled(intptr_t handle, bool *enabled);

/**
 * @brief Enables or Disables the Audio port corresponding to the specified port handle.
 *
 * @param[in] handle   - Handle of the output audio port
 * @param[in] enabled  - Flag to control the audio port state
 *                         ( @a true to enable, @a false to disable)
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsIsAudioPortEnabled()
 */
dsError_t  dsEnableAudioPort(intptr_t handle, bool enabled);

/**
 * @brief Enables or Disables MS12 DAPV2 and DE feature
 * 
 * @param[in] handle   - Handle of the output audio port
 * @param[in] feature  - Enums for MS12 features. Please refer ::dsMS12FEATURE_t
 * @param[in] enable   - Flag to control the MS12 features
 *                         ( @a true to enable, @a false to disable)
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetMS12AudioProfileList(), dsGetMS12AudioProfile()
 */
dsError_t  dsEnableMS12Config(intptr_t handle, dsMS12FEATURE_t feature,const bool enable);

/**
 * @brief Enables or Disables Loudness Equivalence feature.
 *
 * @param[in] handle  - Handle of the output audio port
 * @param[in] enable  - Flag to control the LE features
 *                        ( @a true to enable, @a false to disable)
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetLEConfig()
 */
dsError_t  dsEnableLEConfig(intptr_t handle, const bool enable);

/**
 * @brief Gets the LE (Loudness Equivalence) configuration.
 *
 * This function is used to Get LE (Loudness Equivalence) feature of the audio port corresponding to specified port handle.
 *
 * @param[in] handle   - Handle for the output Audio port
 * @param[out] enable  - Flag which return status of LE features
 *                         ( @a true to enable, @a false to disable)
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsEnableLEConfig()
 */
dsError_t dsGetLEConfig(intptr_t handle, bool *enable);

/**
 * @brief Sets the encoding type of an audio port
 * 
 * This function sets the audio encoding type to be used on the specified audio port.
 *
 * @param[in] handle    - Handle for the output audio port
 * @param[in] encoding  - The encoding type to be used on the audio port. Please refer ::dsAudioEncoding_t
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetAudioEncoding()
 */
dsError_t  dsSetAudioEncoding(intptr_t handle, dsAudioEncoding_t encoding);

/**
 * @brief Sets the audio compression of an audio port.
 * 
 * This function sets the audio compression level(non-MS12) to be used on the audio port corresponding to the specified port handle.
 *
 * @param[in] handle       - Handle for the output audio port
 * @param[in] compression  - Audio compression level (value ranges from 0 to 10) to be used on the audio port
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetAudioCompression()
 */
dsError_t  dsSetAudioCompression(intptr_t handle, int compression);

/**
 * @brief Sets the Dialog Enhancement level of an audio port.
 * 
 * This function sets the dialog enhancement level to be used in the audio port corresponding to specified port handle.
 *
 * @param[in] handle  - Handle for the output audio port.
 * @param[in] level   - Dialog Enhancement level. Level ranges from 0 to 16.
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetDialogEnhancement()
 */
dsError_t  dsSetDialogEnhancement(intptr_t handle, int level);

/**
 * @brief To enable/disable Dolby Volume Mode.
 * 
 * This function sets the dolby audio mode status to the audio port corresponding to port handle.
 *
 * @param[in] handle  - Handle for the output audio port
 * @param[in] mode    - Dolby volume mode. 
 *                        ( @a true to enable Dolby volume mode and @a false to disable Dolby volume mode )
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetDolbyVolumeMode()
 */
dsError_t  dsSetDolbyVolumeMode(intptr_t handle, bool mode);

/**
 * @brief Sets the Intelligent Equalizer Mode.
 * 
 * This function sets the Intelligent Equalizer Mode to be used in the audio port corresponding to the specified port handle.
 *
 * @param[in] handle  - Handle for the output audio port.
 * @param[in] mode    - Intelligent Equalizer mode. 0 = OFF, 1 = Open, 2 = Rich, 3 = Focused,
 *                        4 = Balanced, 5 = Warm, 6 = Detailed
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetIntelligentEqualizerMode()
 */
dsError_t  dsSetIntelligentEqualizerMode(intptr_t handle, int mode);

/**
 * @brief Sets the Dolby volume leveller settings.
 *
 * This function sets the Volume leveller(mode and level) value to be used in the audio port corresponding to specified port handle.
 *
 * @param[in] handle       - Handle for the output Audio port
 * @param[in] volLeveller  - Volume Leveller setting. Please refer ::dsVolumeLeveller_t
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetVolumeLeveller()
 */
dsError_t  dsSetVolumeLeveller(intptr_t handle, dsVolumeLeveller_t volLeveller);

/**
 * @brief Sets the audio Bass
 *
 * This function sets the Bass to be used in the audio port corresponding to specified port handle.
 *
 * @param[in] handle  - Handle for the output Audio port
 * @param[in] boost   - Bass Enhancer boost value (ranging from 0 to 100)
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetBassEnhancer()
 */
dsError_t  dsSetBassEnhancer(intptr_t handle, int boost);

/**
 * @brief Enables / Disables the audio Surround Decoder.
 *
 * This function will enable/disable surround decoder of the audio port corresponding to specified port handle.
 *
 * @param[in] handle   - Handle for the output Audio port
 * @param[in] enabled  - Surround Decoder enabled(1)/disabled(0) value
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsIsSurroundDecoderEnabled()
 */
dsError_t  dsEnableSurroundDecoder(intptr_t handle, bool enabled);

/**
 * @brief Sets the DRC Mode of specified audio port.
 *
 * This function sets the Dynamic Range Control to be used in the audio port corresponding to port handle.
 *
 * @param[in] handle  - Handle for the output Audio port
 * @param[in] mode    - DRC mode (0 for DRC Line Mode and 1 for DRC RF mode)
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetDRCMode()
 */
dsError_t  dsSetDRCMode(intptr_t handle, int mode);

/**
 * @brief Sets the audio Surround Virtualizer level
 *
 * This function sets the Surround Virtualizer level(mode and boost) to be used in the audio port corresponding to specified port handle.
 *
 * @param[in] handle       - Handle for the output Audio port
 * @param[in] virtualizer  - Surround virtualizer setting. Please refer ::dsSurroundVirtualizer_t
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetSurroundVirtualizer()
 */
dsError_t  dsSetSurroundVirtualizer(intptr_t handle, dsSurroundVirtualizer_t virtualizer);

/**
 * @brief Set the Media Intelligent Steering of the audio port.
 *
 * This function sets the enable/disable status of Media Intelligent Steering for the audio port corresponding to specified port handle.
 *
 * @param[in] handle   - Handle for the output Audio port
 * @param[in] enabled  - MI Steering enabled(1)/disabled(0) value 
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetMISteering()
 */
dsError_t  dsSetMISteering(intptr_t handle, bool enabled);

/**
 * @brief Sets the Graphic Equalizer Mode.
 *
 * This function sets the Graphic Equalizer Mode setting to be used in the audio port corresponding to the specified port handle.
 *
 * @param[in] handle  - Handle for the output audio port.
 * @param[in] mode    - Graphic Equalizer mode. 0 for EQ OFF, 1 for EQ Open, 2 for EQ Rich and 3 for EQ Focused
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetGraphicEqualizerMode()
 */
dsError_t  dsSetGraphicEqualizerMode(intptr_t handle, int mode);

/**
 * @brief Sets the MS12 audio profile
 *
 * This function will configure the user selected ms12 audio profile
 *
 * @param[in] handle   - Handle for the output audio port
 * @param[in] profile  - Audio profile to be used from the supported list. Please refer ::_dsMS12AudioProfileList_t
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetMS12AudioProfile(), dsGetMS12AudioProfileList()
 */
dsError_t  dsSetMS12AudioProfile(intptr_t handle, const char* profile);

/**
 * @brief Sets the stereo mode of an audio port. 
 * 
 * This function sets the stereo mode to be used on the audio port corresponding to specified port handle.
 *
 * @param[in] handle  - Handle for the output audio port
 * @param[in] mode    - Stereo mode to be used on the specified audio port. Please refer ::dsAudioStereoMode_t
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetStereoMode()
 */
dsError_t  dsSetStereoMode(intptr_t handle, dsAudioStereoMode_t mode);

/**
 * @brief Sets the Auto Mode to be used on the audio port. 
 * 
 * This function sets the auto mode to be used on the specified audio port.
 *
 * @param[in] handle    - Handle for the output audio port.
 * @param[in] autoMode  - Indicates the auto mode ( @a true if enabled, @a false if disabled ) to be used on audio port.
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetStereoAuto()
 */
dsError_t  dsSetStereoAuto(intptr_t handle, int autoMode);

/**
 * @brief Sets the audio gain of an audio port.
 * 
 * This function sets the gain to be used on the audio port corresponding to specified port handle.
 *
 * @param[in] handle  - Handle for the output audio port
 * @param[in] gain    - Audio Gain to be used on the audio port value
 *                         The Gain ranges between -2080 and 480
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetAudioGain()
 */
dsError_t  dsSetAudioGain(intptr_t handle, float gain);

/**
 * @brief Sets the current audio dB level of an audio port.
 * 
 * This function sets the dB level to be used on the audio port corresponding to specified port handle.
 * Max dB is 180 and Min dB is -1450
 *
 * @param[in] handle  - Handle for the output audio port
 * @param[in] db      - Audio dB level to be used on the audio port
 * 
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetAudioDB()
 */
dsError_t  dsSetAudioDB(intptr_t handle, float db);

/**
 * @brief Sets the audio volume level of an audio port.
 * 
 * This function sets the audio volume level to be used on the audio port corresponding to specified port handle.
 *
 * @param[in] handle  - Handle for the output audio port
 * @param[in] level   - Volume level value (ranging from 0 to 100) to be used on the specified audio port
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetAudioLevel()
 */
dsError_t  dsSetAudioLevel(intptr_t handle, float level);

/**
 * @brief Sets the audio ducking level of an audio port. 
 * 
 * This function sets the audio ducking level to be used on the specified audio port based on the audio output mode. 
 * If output mode is expert mode, this will mute the audio.
 *
 * @param[in] handle  - Handle for the output audio port
 * @param[in] action  - action type to start or stop ducking. Please refer ::dsAudioDuckingAction_t
 * @param[in] type    - ducking type is absolute or relative to current volume level. Please refer ::dsAudioDuckingType_t
 * @param[in] level   - The volume level value from 0 to 100 to be used on the audio port
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 */
dsError_t  dsSetAudioDucking(intptr_t handle, dsAudioDuckingAction_t action, dsAudioDuckingType_t type, const unsigned char level);

/**
 * @brief Sets loop-through mode of an audio port.
 * 
 * This function enables/disables audio loop-through on the audio port corresponding to the specified port handle.
 *
 * @param[in] handle    - Handle for the output audio port
 * @param[in] loopThru  - Flag to enable/disable loop-through
 *                          ( @a true to enable, @a false to disable)
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t  dsEnableLoopThru(intptr_t handle, bool loopThru);

/**
 * @brief Mutes or un-mutes an audio port.
 * 
 * This function mutes or un-mutes the audio port corresponding to the specified port handle.
 * 
 * @param[in] handle  - Handle for the output audio port
 * @param[in] mute    - Flag to mute/un-mute the audio port
 *                        ( @a true to mute, @a false to un-mute)
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsIsAudioMute()
 */
dsError_t  dsSetAudioMute(intptr_t handle, bool mute);

/**
 * @brief Checks whether the audio port supports Dolby MS11 Multistream Decode.
 * 
 * This function checks whether specified audio port supports Dolby MS11 Multistream decode or not.
 *
 * @param[in]  handle         - Handle for the output audio port
 * @param[out] HasMS11Decode  - MS11 Multistream Decode setting for the specified audio port
 *                                ( @a true if audio port supports Dolby MS11 Multistream Decoding or @a false otherwise )
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t  dsIsAudioMSDecode(intptr_t handle, bool *HasMS11Decode);

/**
 * @brief Checks whether the audio port supports Dolby MS12 Multistream Decode.
 * 
 * This function checks whether specified audio port supports Dolby MS12 Multistream decode or not.
 *
 * @param[in] handle          - Handle for the output audio port
 * @param[out] HasMS12Decode  - MS12 Multistream Decode setting
 *                                ( @a true if audio port supports Dolby MS12 Multistream Decoding or @a false otherwise ) 
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 */
dsError_t  dsIsAudioMS12Decode(intptr_t handle, bool *HasMS12Decode);

/**
 * @brief Sets the audio delay (in ms) of an audio port.
 * 
 * This function will set the audio delay (in milliseconds) of audio port corresponding to the specified port handle.
 *
 * @param[in] handle        - Handle for the output Audio port
 * @param[in] audioDelayMs  - Amount of delay(in milliseconds)
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetAudioDelay()
 */
dsError_t dsSetAudioDelay(intptr_t handle, const uint32_t audioDelayMs);

/**
 * @brief Sets the audio delay offset (in ms) of an audio port.
 * 
 * This function will set the audio delay offset (in milliseconds) of the audio port corresponding to specified port handle.
 *
 * @param[in] handle              - Handle for the output Audio port
 * @param[in] audioDelayOffsetMs  - Amount of delay offset(in milliseconds)
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetAudioDelayOffset()
 */
dsError_t dsSetAudioDelayOffset(intptr_t handle, const uint32_t audioDelayOffsetMs);

/**
 * @brief Terminate the Audio Port sub-system of Device Settings HAL.
 * 
 * This function terminates all the audio output ports by releasing the audio port specific handles
 * and the allocated resources.
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t  dsAudioPortTerm();

/**
 * @brief Checks if the audio output port is connected or not.
 *
 * This function is used to check if the audio output port is connected or not.
 *
 * @param[in] handle        - Handle for the output Audio port
 * @param[out] pisCon       - Flag for audio port connection status 
 *                              ( @a true if audio port is connected and @a false if Not Connected)
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsAudioOutIsConnected(intptr_t handle, bool* pisCon);

/**
 * @brief Registers for the Audio Output Connect Event
 *
 * This function is used to register for the Audio Output Connect Event
 *
 * @param[in] CBFunc  - Audio output port connect callback function. Please refer ::dsAudioOutPortConnectCB_t
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsAudioOutRegisterConnectCB(dsAudioOutPortConnectCB_t CBFunc);

/**
 * @brief Registers for the Audio Format Update Event
 *
 * This function is used to register for the Audio Format Update Event
 *
 * @param[in] cbFun  - Audio format update callback function. Please refer ::dsAudioFormatUpdateCB_t
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsAudioFormatUpdateRegisterCB (dsAudioFormatUpdateCB_t cbFun);

/**
 * @brief Register for the Atmos capability change event of the sink device
 *
 * @param[in] cbFun  - Atmos Capability chance callback function. Please refer ::dsAtmosCapsChangeCB_t
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
**/
dsError_t dsAudioAtmosCapsChangeRegisterCB (dsAtmosCapsChangeCB_t cbFun);

/**
 * @brief Gets the Audio Format capabilities .
 * 
 * This function is used to get the supported Audio capabilities for the input port supported by the platform.
 *
 * @param[in]  handle        - Handle for the output audio port 
 * @param[out] capabilities  - Bitwise OR value of supported Audio standards. Please refer ::dsAudioCapabilities_t
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 *
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsGetAudioCapabilities(intptr_t handle, int *capabilities);

/**
 * @brief Gets the MS12 capabilities of audio port supported by the platform.
 * 
 * This function is used to get the supported MS12 capabilities for the input port supported by the platform.
 *
 * @param[in]  handle        - Handle for the output audio port
 * @param[out] capabilities  - OR-ed value of supported MS12 standards. Please refer ::dsMS12Capabilities_t
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsGetMS12Capabilities(intptr_t handle, int *capabilities);

/**
 * @brief Resets the Dialog Enhancement of audio port to default value.
 *
 * This function is used to reset the dialog enhancement of audio port corresponding to the specified port handle to its platform-specific default value.
 *
 * @param[in] handle  - Handle for the output audio port
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsResetDialogEnhancement(intptr_t handle);

/**
 * @brief Resets the audio bass enhancer to its default value.
 *
 * This function is used to reset the audio bass enhancer of audio port corresponding to port handle to its platform-specific default bass boost value.
 *
 * @param[in] handle  - Handle for the output audio port
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 *
 * @see dsGetBassEnhancer, dsSetBassEnhancer
 *
 * @warning  This API is Not thread safe.
 */
dsError_t dsResetBassEnhancer(intptr_t handle);

/**
 * @brief Resets the audio surround virtualizer level to its default value.
 *
 * This function is used to reset the audio surround virtualizer level of audio port corresponding to port handle to its platform-specific default boost value.
 *
 * @param[in] handle  - Handle for the output audio port
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 *
 * @see dsGetSurroundVirtualizer, dsSetSurroundVirtualizer
 *
 * @warning  This API is Not thread safe.
 */
dsError_t dsResetSurroundVirtualizer(intptr_t handle);

/**
 * @brief Resets the Dolby volume leveller of the audio port to its default volume level.
 *
 * This function is used to reset the Dolby volume leveller of audio port corresponding to port handle to its platform-specific default volume level.
 *
 * @param[in] handle  - Handle for the output audio port
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetVolumeLeveller(), dsSetVolumeLeveller()
 * 
 */
dsError_t dsResetVolumeLeveller(intptr_t handle);

/**
 * @brief To Set/override a specific audio setting in 
 *  a specific profile
 *
 * This function will override a specific audio setting in a
 * specific profile
 *
 * @param [in] handle       Handle for the Output Audio port
 * @param [in] *profileState possible values ADD and REMOVE setting from the persistence
 * @param [in] *profileName  ProfileName 
 * @param [in] *profileSettingsName MS12 property name
 * @param [in] *profileSettingValue MS12 property value 
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 */
dsError_t  dsSetMS12AudioProfileSetttingsOverride(intptr_t handle,const char* profileState,const char* profileName,
                                                   const char* profileSettingsName,const char* profileSettingValue);

/**
 * @brief Enables/Disables associated audio mixing feature.
 *
 * This function will enable/disable associated audio mixing feature of audio port corresponding to specified port handle.
 *
 * @param[in] handle  - Handle for the output audio port
 * @param[in] mixing  - Flag to control audio mixing feature
 *                        ( @a true to enable, @a false to disable)
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetAssociatedAudioMixing()
 */
dsError_t dsSetAssociatedAudioMixing(intptr_t handle, bool mixing);

/**
 * @brief Gets the Associated Audio Mixing status - enabled/disabled
 *
 * This function is used to get the audio mixing status(enabled/disabled) of audio port corresponding to specified port handle.
 *
 * @param[in] handle   - Handle for the output Audio port
 * @param[out] mixing  - Associated Audio Mixing status
 *                         ( @a true if enabled and @a false if disabled)
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetAssociatedAudioMixing()
 */
dsError_t  dsGetAssociatedAudioMixing(intptr_t handle, bool *mixing);

/**
 * @brief Sets the mixerbalance between main and associated audio
 *
 * This function will set the mixerbalance between main and associated audio of audio port corresponding to specified port handle.
 *
 * @param[in] handle        - Handle for the output Audio port
 * @param[in] mixerbalance  - int value -32(mute associated audio) to +32(mute main audio)
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetFaderControl()
 */
dsError_t  dsSetFaderControl(intptr_t handle, int mixerbalance);

/**
 * @brief To get the mixer balance between main and associated audio
 *
 * This function will get the mixer balance between main and associated audio of audio port corresponding to specified port handle.
 *
 * @param[in]  handle        - Handle for the output Audio port
 * @param[out] mixerbalance  - int value -32(mute associated audio) to +32(mute main audio)
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetFaderControl()
 */
dsError_t  dsGetFaderControl(intptr_t handle, int* mixerbalance);

/**
 * @brief Sets AC4 Primary language
 *
 * This function will set AC4 Primary language of audio port corresponding to specified port handle.
 *
 * @param[in] handle  - Handle for the output Audio port
 * @param[in] pLang   - char* 3 letter language code string as per ISO 639-3
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetPrimaryLanguage()
 */
dsError_t  dsSetPrimaryLanguage(intptr_t handle, const char* pLang);

/**
 * @brief To get AC4 Primary language
 *
 * This function will get AC4 Primary language of audio port corresponding to specified port handle.
 *
 * @param[in] handle  - Handle for the output Audio port
 * @param[out] pLang  - char* 3 letter lang code should be used as per ISO 639-3
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetPrimaryLanguage()
 */
dsError_t  dsGetPrimaryLanguage(intptr_t handle, char* pLang);

/**
 * @brief To set AC4 Secondary language
 *
 * This function will set AC4 Secondary language of audio port corresponding to specified port handle.
 *
 * @param[in] handle  - Handle for the output Audio port (Not Used as setting is not port specific)
 * @param[in] sLang   - char* 3 letter lang code should be used as per ISO 639-3
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetSecondaryLanguage()
 */
dsError_t  dsSetSecondaryLanguage(intptr_t handle, const char* sLang);

/**
 * @brief Gets the AC4 Secondary language
 *
 * This function will get AC4 Secondary language of audio port corresponding to specified port handle.
 *
 * @param[in] handle  - Handle for the output Audio port (Not Used as setting is not port specific)
 * @param[out] sLang  - char* 3 letter lang code should be used as per ISO 639-3
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() and dsGetAudioPort() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetSecondaryLanguage()
 */
dsError_t  dsGetSecondaryLanguage(intptr_t handle, char* sLang);

/**
 * @brief Gets the audio HDMI ARC port ID for each platform
 *
 * This function will get audio HDMI ARC port ID of platform
 *
 * @param[in] portId  - HDMI ARC port ID
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre  dsAudioPortInit() should be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetSupportedARCTypes()
 */
dsError_t dsGetHDMIARCPortId(int *portId);

#ifdef __cplusplus
}
#endif
#endif /* _DS_AUDIOOUTPORT_H_ */

/** @} */ // End of DS HAL Audio Public APIs
/** @} */ // End of DS Audio HAL
/** @} */ // End of Device Settings HAL
/** @} */ // End of Device Settings Module
/** @} */ // End of HPK