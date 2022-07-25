#pragma once

/**
 * @brief modulo, properly
 * 
 * @param a dividend
 * @param b divisor
 * @return int 
 */
int Mod(int a, int b);

/**
 * @brief floor integer division
 * 
 * @param a divd
 * @param b divsor
 * @return int res
 */
int FloorDiv(int a, int b);

/**
 * @brief convert a world coordinate for a block to a chunk coordinate
 * 
 * @param x 
 * @return int 
 */
int ChunkMod(int x);

/**
 * @brief convert a world coordinate for a block to a world coordinate for a chunk
 * 
 * @param x 
 * @return int 
 */
int ChunkFloor(int x);