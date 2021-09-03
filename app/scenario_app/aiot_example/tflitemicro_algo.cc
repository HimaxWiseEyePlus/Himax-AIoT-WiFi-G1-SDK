/*
 * tflitemicro_algo.cc
 *
 *  Created on: 2020¦~5¤ë27¤é
 *      Author: 902447
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_error.h"
#include "board_config.h"
#include "arc_timer.h"


#if defined(TFLITE_MICRO_GOOGLE_PERSON)
#include <library/cv/tflitemicro_25/tensorflow/lite/c/common.h>
#include <library/cv/tflitemicro_25/tensorflow/lite/micro/kernels/micro_ops.h>
#include <library/cv/tflitemicro_25/tensorflow/lite/micro/micro_error_reporter.h>
#include <library/cv/tflitemicro_25/tensorflow/lite/micro/micro_interpreter.h>
#include <library/cv/tflitemicro_25/tensorflow/lite/micro/micro_mutable_op_resolver.h>
//#include <library/cv/tflitemicro_25/tensorflow/lite/micro/testing/micro_test.h>
#include <library/cv/tflitemicro_25/tensorflow/lite/schema/schema_generated.h>
#include <library/cv/tflitemicro_25/tensorflow/lite/version.h>
#endif
#include "tflitemicro_algo.h"


#ifdef TFLITE_MICRO_GOOGLE_PERSON
extern const unsigned char g_person_detect_model_data[];
extern const int g_person_detect_model_data_len;

constexpr int tensor_arena_size = 125*1024;
#ifdef __GNU__
	uint8_t tensor_arena[tensor_arena_size] __attribute__((aligned(16)));
#else
	#pragma Bss(".tensor_arena")
	uint8_t tensor_arena[tensor_arena_size];
	#pragma Bss()
#endif

#define HIMAX_INPUT_SIZE_X (96)
#define HIMAX_INPUT_SIZE_Y (96)


#endif


#if defined(TFLITE_MICRO_GOOGLE_PERSON)
namespace {
tflite::MicroErrorReporter micro_error_reporter;
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
}

//TFu debug log, make it hardware platform print
extern "C" void DebugLog(const char* s) {xprintf("%s", s);}//{ fprintf(stderr, "%s", s); }

#define LOCAL_FRAQ_BITS (8)
#define SC(A, B) ((A<<8)/B)
void fd_rescale(
        const uint8_t*in_image,
        const int32_t width,
        const int32_t height,
        const int32_t nwidth,
        const int32_t nheight,
        uint8_t*out_image,
        const int32_t nxfactor,
        const int32_t nyfactor) {
    int32_t x,y;
    int32_t ceil_x, ceil_y, floor_x, floor_y;

    int32_t fraction_x,fraction_y,one_min_x,one_min_y;
    int32_t pix[4];//4 pixels for the bilinear interpolation
    int32_t out_image_fix;

    for (y = 0; y < nheight; y++) {//compute new pixels
        for (x = 0; x < nwidth; x++) {
            floor_x = (x*nxfactor) >> LOCAL_FRAQ_BITS;//left pixels of the window
            floor_y = (y*nyfactor) >> LOCAL_FRAQ_BITS;//upper pixels of the window

            ceil_x = floor_x+1;//right pixels of the window
            if (ceil_x >= width) ceil_x=floor_x;//stay in image

            ceil_y = floor_y+1;//bottom pixels of the window
            if (ceil_y >= height) ceil_y=floor_y;

            fraction_x = x*nxfactor-(floor_x << LOCAL_FRAQ_BITS);//strength coefficients
            fraction_y = y*nyfactor-(floor_y << LOCAL_FRAQ_BITS);

            one_min_x = (1 << LOCAL_FRAQ_BITS)-fraction_x;
            one_min_y = (1 << LOCAL_FRAQ_BITS)-fraction_y;

            pix[0] = in_image[floor_y * width + floor_x];//store window
            pix[1] = in_image[floor_y * width + ceil_x];
            pix[2] = in_image[ceil_y * width + floor_x];
            pix[3] = in_image[ceil_y * width + ceil_x];

            //interpolate new pixel and truncate it's integer part
            out_image_fix = one_min_y*(one_min_x*pix[0]+fraction_x*pix[1])+fraction_y*(one_min_x*pix[2]+fraction_x*pix[3]);
            out_image_fix = out_image_fix >> (LOCAL_FRAQ_BITS * 2);
            out_image[nwidth*y+x] = out_image_fix;
        }
    }
}
#endif

extern "C" int tflitemicro_algo_init()
{
	int ercode = 0;
#ifdef TFLITE_MICRO_GOOGLE_PERSON

	error_reporter = &micro_error_reporter;
	error_reporter->Report("start Himax..\n");
	model = ::tflite::GetModel(g_person_detect_model_data);
	if (model->version() != TFLITE_SCHEMA_VERSION) {
		error_reporter->Report(
			"Model provided is schema version %d not equal "
			"to supported version %d.\n",
			model->version(), TFLITE_SCHEMA_VERSION);
	}


	static tflite::MicroMutableOpResolver<5> micro_op_resolver;
	micro_op_resolver.AddAveragePool2D();
	micro_op_resolver.AddConv2D();
	micro_op_resolver.AddDepthwiseConv2D();
	micro_op_resolver.AddReshape();
	micro_op_resolver.AddSoftmax();


	// Build an interpreter to run the model with.
	static tflite::MicroInterpreter static_interpreter(model, micro_op_resolver,
										   tensor_arena, tensor_arena_size,
										   error_reporter);
	interpreter = &static_interpreter;
	if(interpreter->AllocateTensors()!= kTfLiteOk) {
		ercode = -1;
	}
#endif
	return ercode;
}

extern "C" int tflitemicro_algo_run(uint32_t image_addr, uint32_t image_width, uint32_t image_height, struct_algoResult *algoresult)
{
	int ercode = 0;
#ifdef USE_TICK
	uint32_t tick_start = 0, tick_stop = 0;
#endif
#ifdef TFLITE_MICRO_GOOGLE_PERSON


	TfLiteTensor* input = interpreter->input(0);
	fd_rescale((uint8_t*)image_addr, image_width, image_height, HIMAX_INPUT_SIZE_X, HIMAX_INPUT_SIZE_Y,
			input->data.uint8, SC(image_width, HIMAX_INPUT_SIZE_X), SC(image_height, HIMAX_INPUT_SIZE_Y));

	// Now test with a blank image.
	  for (int i = 0; i < input->bytes; ++i) {
	    //input->data.uint8[i] = himax_input_image[i];
	    input->data.int8[i] = input->data.uint8[i] - 128;

	  }

#ifdef USE_TICK
	timer_stop(TIMER_0);
	timer_start(TIMER_0, TIMER_CTRL_NH, 0xffffffff);			// Set Counter LIMIT to MAX
	tick_start = _arc_aux_read(AUX_TIMER0_CNT);
//	dbg_printf(DBG_LESS_INFO,"TFStart tick_start=%d\n",tick_start);
#endif
	TfLiteStatus invoke_status = interpreter->Invoke();
#ifdef USE_TICK
	tick_stop = _arc_aux_read(AUX_TIMER0_CNT);
#endif
	if(invoke_status != kTfLiteOk)
	{
		error_reporter->Report("invoke fail\n");
	}
	//error_reporter->Report("invoke period=%d\n", tick_stop- tick_start);


	TfLiteTensor* output = interpreter->output(0);

	int8_t person_score = output->data.int8[1];
	int8_t no_person_score = output->data.int8[0];
#if 1
	error_reporter->Report(
	       "person score: %d, no person score: %d\n", person_score,
	       no_person_score);
#else
	error_reporter->Report(
	       "score: %d\n", person_score);
#endif
	if(person_score > 0)
	{
		algoresult->humanPresence = true;
		#ifdef SCALABLE_METADATA
			algoresult->bd.num_of_detection = 1;
		#else
			algoresult->num_tracked_human_targets = 1;
		#endif
	}else{
		algoresult->humanPresence = false;
		#ifdef SCALABLE_METADATA
			algoresult->bd.num_of_detection = 0;
		#else
			algoresult->num_tracked_human_targets = 0;
		#endif
	}
#endif
	return ercode;
}

void tflitemicro_algo_exit()
{
	hx_lib_pm_cplus_deinit();
}
