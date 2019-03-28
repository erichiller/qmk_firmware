/* Copyright 2017 Fred Sundvik
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// #ifdef LED_MATRIX
#ifndef KEYBOARDS_WHITEFOX_SIMPLE_VISUALIZER_H_
#define KEYBOARDS_WHITEFOX_SIMPLE_VISUALIZER_H_


// #ifdef LED_MATRIX_XXX
#include "visualizer.h"
#include "visualizer_keyframes.h"
#include "led.h"
#include "default_animations.h"
// #endif

#include "print.h"
// #include "debug.h"

// #ifdef LED_MATRIX_XXX
static bool initial_update = true;
// #endif

// Feel free to modify the animations below, or even add new ones if needed

void initialize_user_visualizer(visualizer_state_t* state) {
    print("visualizer.c initialize_user_visualizer\n");
    // The brightness will be dynamically adjustable in the future
    // But for now, change it here.
    
// #ifdef LED_MATRIX_XXX
    initial_update = true;
    // print("initialize_user_visualizer");
    start_keyframe_animation(&default_startup_animation);
// #endif
}


void update_user_visualizer_state(visualizer_state_t* state, visualizer_keyboard_status_t* prev_status) {
    print("visualizer.c update_user_visualizer_state\n");
    // Add more tests, change the colors and layer texts here
    // Usually you want to check the high bits (higher layers first)
    // because that's the order layers are processed for keypresses
    // You can for check for example:
    // state->status.layer
    // state->status.default_layer
    // state->status.leds (see led.h for available statuses)
    // print("update_user_visualizer_state");

// #ifdef LED_MATRIX_XXX
    if (initial_update) { initial_update=false; start_keyframe_animation(&led_test_animation); }
// #endif
}


void user_visualizer_suspend(visualizer_state_t* state) {
    print("visualizer.c user_visualizer_suspend\n");

// #ifdef LED_MATRIX_XXX
    start_keyframe_animation(&default_suspend_animation);
// #endif
}

void user_visualizer_resume(visualizer_state_t* state) {
    print("visualizer.c user_visualizer_resume\n");
    
// #ifdef LED_MATRIX_XXX
    initial_update = true;
    start_keyframe_animation(&default_startup_animation);
// #endif
}

#endif /* KEYBOARDS_WHITEFOX_SIMPLE_VISUALIZER_H_ */
// #endif