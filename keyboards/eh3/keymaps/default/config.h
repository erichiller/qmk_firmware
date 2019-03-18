
// // place overrides here

// /* We only use CTL_T style mod-tap keys, so we want the built-in
//  * mod-tap-interrupt behavior to allow quickly typing CTRL-W arrow
//  * sequences with a CTL_T(ESC) key */
// #undef IGNORE_MOD_TAP_INTERRUPT

// /* We want our layer toggle tap to be a double tap */
// #define TAPPING_TOGGLE 2





// #define ROW_SHIFTER ((matrix_row_t)1)

// #define MCP_GP_BASE 0x70

// #define GPB0 (MCP_GP_BASE + 0)
// #define GPB1 (MCP_GP_BASE + 1)
// #define GPB2 (MCP_GP_BASE + 2)
// #define GPB3 (MCP_GP_BASE + 3)
// #define GPB4 (MCP_GP_BASE + 4)
// #define GPB5 (MCP_GP_BASE + 5)
// #define GPB6 (MCP_GP_BASE + 6)
// #define GPB7 (MCP_GP_BASE + 7)
// #define GPA0 (MCP_GP_BASE + 0x08 + 0)
// #define GPA1 (MCP_GP_BASE + 0x08 + 1)
// #define GPA2 (MCP_GP_BASE + 0x08 + 2)
// #define GPA3 (MCP_GP_BASE + 0x08 + 3)
// #define GPA4 (MCP_GP_BASE + 0x08 + 4)
// #define GPA5 (MCP_GP_BASE + 0x08 + 5)
// #define GPA6 (MCP_GP_BASE + 0x08 + 6)
// #define GPA7 (MCP_GP_BASE + 0x08 + 7)

// #define ANL0  F7
// #define ANL1  F6
// #define ANL2  F5
// #define ANL3  F4
// #define ANL4  F1
// #define ANL5  F0
// #define ANL7  D7
// #define ANL9  B5
// #define ANL10 B6
// #define ANL11 D6


// // #define MATRIX_ROW_PINS { ANL0 , ANL1 , ANL2 , ANL3 , ANL4 , ANL5 }
// #define MATRIX_ROW_PINS { ANL5 , ANL4 , ANL3 , ANL2 , ANL1 , ANL0 }

// //                        C=0    C=1    C=2    C=3    C=4    C=5    C=6    C=7    C=8   C=9   C=10   C=11   C=12   C=13   C=14   C=15   C=16   C=17   C=18 //
// #define MATRIX_COL_PINS { GPA2 , GPA3 , GPA1 , GPB4 , GPB5 , GPB7 , GPB6 , D7   , B5  , B6  , GPA4 , D6   , GPB0 , GPA7 , GPB1 , GPA6 , GPB2 , GPA5 , GPB3 }
