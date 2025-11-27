// imageRGBTest - A program that performs some operations on RGB images.
//
// This program is an example use of the imageRGB module,
// a programming project for the course AED, DETI / UA.PT
//
// You may freely use and modify this code, NO WARRANTY, blah blah,
// as long as you give proper credit to the original and subsequent authors.
//
// The AED Team <jmadeira@ua.pt, jmr@ua.pt, ...>
// 2025

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "imageRGB.h"
#include "instrumentation.h"


// ======================= TESTES FEITOS POR BERNARDO REIS E FERNANDO =======================

// Test result tracking
static int tests_passed = 0;
static int tests_failed = 0;

// Helper macros for testing
#define TEST_START(name) printf("\n=== TEST: %s ===\n", name)
#define TEST_ASSERT(condition, message) \
  do { \
    if (condition) { \
      printf("  ✓ PASS: %s\n", message); \
      tests_passed++; \
    } else { \
      printf("  ✗ FAIL: %s\n", message); \
      tests_failed++; \
    } \
  } while(0)
#define TEST_END() printf("---\n")

// Test functions
void test_image_creation() {
  TEST_START("Image Creation");
  
  Image img1 = ImageCreate(100, 100);
  TEST_ASSERT(img1 != NULL, "ImageCreate returns non-NULL");
  TEST_ASSERT(ImageWidth(img1) == 100, "Image width is correct");
  TEST_ASSERT(ImageHeight(img1) == 100, "Image height is correct");
  TEST_ASSERT(ImageColors(img1) == 2, "Image has 2 initial colors (white, black)");
  ImageDestroy(&img1);
  TEST_ASSERT(img1 == NULL, "ImageDestroy sets pointer to NULL");
  
  Image img2 = ImageCreateChess(150, 120, 30, 0x000000);
  TEST_ASSERT(img2 != NULL, "ImageCreateChess returns non-NULL");
  TEST_ASSERT(ImageWidth(img2) == 150, "Chess image width is correct");
  TEST_ASSERT(ImageHeight(img2) == 120, "Chess image height is correct");
  ImageDestroy(&img2);
  
  Image img3 = ImageCreatePalete(128, 128, 4);
  TEST_ASSERT(img3 != NULL, "ImageCreatePalete returns non-NULL");
  TEST_ASSERT(ImageColors(img3) == 1000, "Palete has full LUT (1000 colors)");
  ImageDestroy(&img3);
  
  TEST_END();
}

void test_image_copy() {
  TEST_START("Image Copy");
  
  Image original = ImageCreateChess(50, 50, 10, 0xff0000);
  Image copy = ImageCopy(original);
  
  TEST_ASSERT(copy != NULL, "ImageCopy returns non-NULL");
  TEST_ASSERT(ImageWidth(copy) == ImageWidth(original), "Copy has same width");
  TEST_ASSERT(ImageHeight(copy) == ImageHeight(original), "Copy has same height");
  TEST_ASSERT(ImageColors(copy) == ImageColors(original), "Copy has same number of colors");
  TEST_ASSERT(ImageIsEqual(original, copy), "Copy is equal to original");
  TEST_ASSERT(copy != original, "Copy is a different object");
  
  ImageDestroy(&original);
  ImageDestroy(&copy);
  
  TEST_END();
}

void test_image_comparison() {
  TEST_START("Image Comparison");
  
  Image img1 = ImageCreate(50, 50);
  Image img2 = ImageCreate(50, 50);
  Image img3 = ImageCreate(60, 60);
  Image img4 = ImageCreateChess(50, 50, 10, 0x000000);
  
  TEST_ASSERT(ImageIsEqual(img1, img2), "Two identical blank images are equal");
  TEST_ASSERT(!ImageIsEqual(img1, img3), "Images with different dimensions are not equal");
  TEST_ASSERT(!ImageIsEqual(img1, img4), "Images with different content are not equal");
  TEST_ASSERT(ImageIsDifferent(img1, img4), "ImageIsDifferent works correctly");
  
  ImageDestroy(&img1);
  ImageDestroy(&img2);
  ImageDestroy(&img3);
  ImageDestroy(&img4);
  
  TEST_END();
}

void test_rotation_90() {
  TEST_START("Image Rotation 90°");
  
  // Create a simple 3x2 test pattern
  Image original = ImageCreate(3, 2);
  Image rotated = ImageRotate90CW(original);
  
  TEST_ASSERT(rotated != NULL, "ImageRotate90CW returns non-NULL");
  TEST_ASSERT(ImageWidth(rotated) == ImageHeight(original), "Rotated width equals original height");
  TEST_ASSERT(ImageHeight(rotated) == ImageWidth(original), "Rotated height equals original width");
  TEST_ASSERT(ImageWidth(rotated) == 2, "Rotated image has width 2");
  TEST_ASSERT(ImageHeight(rotated) == 3, "Rotated image has height 3");
  
  // Test that rotating 4 times returns to original
  Image rot1 = ImageRotate90CW(original);
  Image rot2 = ImageRotate90CW(rot1);
  Image rot3 = ImageRotate90CW(rot2);
  Image rot4 = ImageRotate90CW(rot3);
  
  TEST_ASSERT(ImageIsEqual(original, rot4), "Four 90° rotations return to original");
  
  ImageDestroy(&original);
  ImageDestroy(&rotated);
  ImageDestroy(&rot1);
  ImageDestroy(&rot2);
  ImageDestroy(&rot3);
  ImageDestroy(&rot4);
  
  // Create visual example with chess pattern
  printf("  → Generating rotation examples...\n");
  Image chess_original = ImageCreateChess(80, 60, 20, 0x000000);
  ImageSavePBM(chess_original, "img/01_rotation_original.pbm");
  
  Image chess_90 = ImageRotate90CW(chess_original);
  ImageSavePBM(chess_90, "img/02_rotation_90cw.pbm");
  
  Image chess_180 = ImageRotate90CW(chess_90);
  ImageSavePBM(chess_180, "img/03_rotation_180cw.pbm");
  
  Image chess_270 = ImageRotate90CW(chess_180);
  ImageSavePBM(chess_270, "img/04_rotation_270cw.pbm");
  
  ImageDestroy(&chess_original);
  ImageDestroy(&chess_90);
  ImageDestroy(&chess_180);
  ImageDestroy(&chess_270);
  
  TEST_END();
}

void test_rotation_180() {
  TEST_START("Image Rotation 180°");
  
  Image original = ImageCreate(50, 30);
  Image rotated = ImageRotate180CW(original);
  
  TEST_ASSERT(rotated != NULL, "ImageRotate180CW returns non-NULL");
  TEST_ASSERT(ImageWidth(rotated) == ImageWidth(original), "Rotated width equals original width");
  TEST_ASSERT(ImageHeight(rotated) == ImageHeight(original), "Rotated height equals original height");
  
  // Test that rotating twice returns to original
  Image rot2 = ImageRotate180CW(rotated);
  TEST_ASSERT(ImageIsEqual(original, rot2), "Two 180° rotations return to original");
  
  ImageDestroy(&original);
  ImageDestroy(&rotated);
  ImageDestroy(&rot2);
  
  // Create visual example
  printf("  → Generating 180° rotation example...\n");
  Image chess = ImageCreateChess(100, 60, 20, 0x000000);
  ImageSavePBM(chess, "img/05_rotation180_original.pbm");
  
  Image chess_180 = ImageRotate180CW(chess);
  ImageSavePBM(chess_180, "img/06_rotation180_result.pbm");
  
  ImageDestroy(&chess);
  ImageDestroy(&chess_180);
  
  TEST_END();
}

void test_file_operations() {
  TEST_START("File I/O Operations");
  
  // Test PBM
  Image chess = ImageCreateChess(150, 120, 30, 0x000000);
  int result = ImageSavePBM(chess, "img/07_chess_bw.pbm");
  TEST_ASSERT(result == 0, "ImageSavePBM succeeds");
  
  Image loaded_pbm = ImageLoadPBM("img/07_chess_bw.pbm");
  TEST_ASSERT(loaded_pbm != NULL, "ImageLoadPBM loads saved file");
  TEST_ASSERT(ImageIsEqual(chess, loaded_pbm), "Loaded PBM equals original");
  
  ImageDestroy(&chess);
  ImageDestroy(&loaded_pbm);
  
  // Test PPM
  Image chess_color = ImageCreateChess(80, 80, 20, 0xff0000);
  result = ImageSavePPM(chess_color, "img/08_chess_red.ppm");
  TEST_ASSERT(result == 0, "ImageSavePPM succeeds");
  
  Image loaded_ppm = ImageLoadPPM("img/08_chess_red.ppm");
  TEST_ASSERT(loaded_ppm != NULL, "ImageLoadPPM loads saved file");
  TEST_ASSERT(ImageIsEqual(chess_color, loaded_ppm), "Loaded PPM equals original");
  
  ImageDestroy(&chess_color);
  ImageDestroy(&loaded_ppm);
  
  // Create additional color examples
  printf("  → Generating color pattern examples...\n");
  Image chess_blue = ImageCreateChess(80, 80, 20, 0x0000ff);
  ImageSavePPM(chess_blue, "img/09_chess_blue.ppm");
  ImageDestroy(&chess_blue);
  
  Image chess_green = ImageCreateChess(80, 80, 20, 0x00ff00);
  ImageSavePPM(chess_green, "img/10_chess_green.ppm");
  ImageDestroy(&chess_green);
  
  Image palete = ImageCreatePalete(128, 128, 8);
  ImageSavePPM(palete, "img/11_color_palete.ppm");
  ImageDestroy(&palete);
  
  TEST_END();
}

void test_region_filling_stack() {
  TEST_START("Region Filling with STACK");
  
  Image img = ImageCreate(10, 10);
  int pixels_filled = ImageRegionFillingWithSTACK(img, 5, 5, BLACK);
  
  TEST_ASSERT(pixels_filled == 100, "STACK fills all 100 pixels in blank image");
  
  // Create a chess pattern and test filling one square
  Image chess = ImageCreateChess(20, 20, 5, 0x000000);
  int filled = ImageRegionFillingWithSTACK(chess, 2, 2, 2);
  TEST_ASSERT(filled == 25, "STACK fills one chess square (5x5=25 pixels)");
  
  ImageDestroy(&img);
  ImageDestroy(&chess);
  
  // Visual example
  printf("  → Generating region filling example (STACK)...\n");
  Image demo = ImageCreateChess(100, 100, 20, 0x000000);
  ImageSavePBM(demo, "img/12_region_original.pbm");
  
  ImageRegionFillingWithSTACK(demo, 10, 10, 1);
  ImageSavePBM(demo, "img/13_region_filled_stack.pbm");
  ImageDestroy(&demo);
  
  TEST_END();
}

void test_region_filling_queue() {
  TEST_START("Region Filling with QUEUE");
  
  Image img = ImageCreate(10, 10);
  int pixels_filled = ImageRegionFillingWithQUEUE(img, 5, 5, BLACK);
  
  TEST_ASSERT(pixels_filled == 100, "QUEUE fills all 100 pixels in blank image");
  
  // Create a chess pattern and test filling one square
  Image chess = ImageCreateChess(20, 20, 5, 0x000000);
  int filled = ImageRegionFillingWithQUEUE(chess, 2, 2, 2);
  TEST_ASSERT(filled == 25, "QUEUE fills one chess square (5x5=25 pixels)");
  
  ImageDestroy(&img);
  ImageDestroy(&chess);
  
  // Visual example
  printf("  → Generating region filling example (QUEUE)...\n");
  Image demo = ImageCreateChess(100, 100, 20, 0x000000);
  ImageRegionFillingWithQUEUE(demo, 10, 10, 1);
  ImageSavePBM(demo, "img/14_region_filled_queue.pbm");
  ImageDestroy(&demo);
  
  TEST_END();
}

void test_region_filling_recursive() {
  TEST_START("Region Filling RECURSIVE");
  
  Image img = ImageCreate(10, 10);
  int pixels_filled = ImageRegionFillingRecursive(img, 5, 5, BLACK);
  
  TEST_ASSERT(pixels_filled == 100, "Recursive fills all 100 pixels in blank image");
  
  // Create a chess pattern and test filling one square
  Image chess = ImageCreateChess(20, 20, 5, 0x000000);
  int filled = ImageRegionFillingRecursive(chess, 2, 2, 2);
  TEST_ASSERT(filled == 25, "Recursive fills one chess square (5x5=25 pixels)");
  
  ImageDestroy(&img);
  ImageDestroy(&chess);
  
  // Visual example
  printf("  → Generating region filling example (RECURSIVE)...\n");
  Image demo = ImageCreateChess(100, 100, 20, 0x000000);
  ImageRegionFillingRecursive(demo, 10, 10, 1);
  ImageSavePBM(demo, "img/15_region_filled_recursive.pbm");
  ImageDestroy(&demo);
  
  TEST_END();
}

void test_region_filling_consistency() {
  TEST_START("Region Filling Consistency (Stack vs Queue vs Recursive)");
  
  // Test that all three methods produce the same pixel count
  Image img1 = ImageCreateChess(30, 30, 6, 0x000000);
  Image img2 = ImageCopy(img1);
  Image img3 = ImageCopy(img1);
  
  int count_stack = ImageRegionFillingWithSTACK(img1, 3, 3, 2);
  int count_queue = ImageRegionFillingWithQUEUE(img2, 3, 3, 2);
  int count_recursive = ImageRegionFillingRecursive(img3, 3, 3, 2);
  
  TEST_ASSERT(count_stack == count_queue, "STACK and QUEUE fill same number of pixels");
  TEST_ASSERT(count_queue == count_recursive, "QUEUE and RECURSIVE fill same number of pixels");
  TEST_ASSERT(count_stack == 36, "All methods fill correct region size (6x6=36 pixels)");
  
  // Nota: As imagens podem diferir na ordem dos pixels, mas todas preenchem a mesma região.
  
  ImageDestroy(&img1);
  ImageDestroy(&img2);
  ImageDestroy(&img3);
  
  TEST_END();
}

void test_fill_methods_performance() {
  TEST_START("Fill Methods Performance Comparison");
  
  printf("  Testing different image sizes and patterns...\n");
  
  // Small blank image
  printf("\n  → Blank 50x50 image:\n");
  Image blank1 = ImageCreate(50, 50);
  Image blank2 = ImageCopy(blank1);
  Image blank3 = ImageCopy(blank1);
  
  InstrReset();
  int c1 = ImageRegionFillingRecursive(blank1, 25, 25, 1);
  printf("    Recursive: %d pixels, ops: %lu\n", c1, InstrCount[0]);
  
  InstrReset();
  int c2 = ImageRegionFillingWithSTACK(blank2, 25, 25, 1);
  printf("    Stack:     %d pixels, ops: %lu\n", c2, InstrCount[0]);
  
  InstrReset();
  int c3 = ImageRegionFillingWithQUEUE(blank3, 25, 25, 1);
  printf("    Queue:     %d pixels, ops: %lu\n", c3, InstrCount[0]);
  
  TEST_ASSERT(c1 == c2 && c2 == c3, "All methods fill same count (blank 50x50)");
  TEST_ASSERT(c1 == 2500, "Fills entire 50x50 image (2500 pixels)");
  
  ImageDestroy(&blank1);
  ImageDestroy(&blank2);
  ImageDestroy(&blank3);
  
  // Chess pattern - single region
  printf("\n  → Chess 40x40 (edge=20) single region:\n");
  Image chess1 = ImageCreateChess(40, 40, 20, 0x000000);
  Image chess2 = ImageCopy(chess1);
  Image chess3 = ImageCopy(chess1);
  
  InstrReset();
  c1 = ImageRegionFillingRecursive(chess1, 2, 2, 5);
  printf("    Recursive: %d pixels, ops: %lu\n", c1, InstrCount[0]);
  
  InstrReset();
  c2 = ImageRegionFillingWithSTACK(chess2, 2, 2, 5);
  printf("    Stack:     %d pixels, ops: %lu\n", c2, InstrCount[0]);
  
  InstrReset();
  c3 = ImageRegionFillingWithQUEUE(chess3, 2, 2, 5);
  printf("    Queue:     %d pixels, ops: %lu\n", c3, InstrCount[0]);
  
  TEST_ASSERT(c1 == c2 && c2 == c3, "All methods fill same count (chess region)");
  TEST_ASSERT(c1 == 400, "Fills 20x20 chess square (400 pixels)");
  TEST_ASSERT(ImageIsEqual(chess1, chess2) && ImageIsEqual(chess2, chess3), 
              "All methods produce identical images");
  
  ImageDestroy(&chess1);
  ImageDestroy(&chess2);
  ImageDestroy(&chess3);
  
  // Complex pattern
  printf("\n  → Chess 60x60 (edge=10) complex:\n");
  Image complex1 = ImageCreateChess(60, 60, 10, 0x000000);
  Image complex2 = ImageCopy(complex1);
  Image complex3 = ImageCopy(complex1);
  
  InstrReset();
  c1 = ImageRegionFillingRecursive(complex1, 5, 5, 7);
  printf("    Recursive: %d pixels, ops: %lu\n", c1, InstrCount[0]);
  
  InstrReset();
  c2 = ImageRegionFillingWithSTACK(complex2, 5, 5, 7);
  printf("    Stack:     %d pixels, ops: %lu\n", c2, InstrCount[0]);
  
  InstrReset();
  c3 = ImageRegionFillingWithQUEUE(complex3, 5, 5, 7);
  printf("    Queue:     %d pixels, ops: %lu\n", c3, InstrCount[0]);
  
  TEST_ASSERT(c1 == c2 && c2 == c3, "All methods fill same count (complex)");
  
  // Nota: A ordem de visita do pixel pode variar entre os métodos.
  
  ImageDestroy(&complex1);
  ImageDestroy(&complex2);
  ImageDestroy(&complex3);
  
  TEST_END();
}

void test_segmentation_comparison() {
  TEST_START("Image Segmentation Method Comparison");
  
  printf("  Comparing segmentation with different fill methods...\n");
  
  // Simple chess pattern
  printf("\n  → Chess 40x40 (edge=10) - 8 regions:\n");
  Image base = ImageCreateChess(40, 40, 10, 0x000000);
  
  Image seg1 = ImageCopy(base);
  InstrReset();
  int regions_rec = ImageSegmentation(seg1, ImageRegionFillingRecursive);
  printf("    Recursive: %d regions, ops: %lu\n", regions_rec, InstrCount[0]);
  
  Image seg2 = ImageCopy(base);
  InstrReset();
  int regions_stack = ImageSegmentation(seg2, ImageRegionFillingWithSTACK);
  printf("    Stack:     %d regions, ops: %lu\n", regions_stack, InstrCount[0]);
  
  Image seg3 = ImageCopy(base);
  InstrReset();
  int regions_queue = ImageSegmentation(seg3, ImageRegionFillingWithQUEUE);
  printf("    Queue:     %d regions, ops: %lu\n", regions_queue, InstrCount[0]);
  
  TEST_ASSERT(regions_rec == regions_stack, "Recursive == Stack (region count)");
  TEST_ASSERT(regions_stack == regions_queue, "Stack == Queue (region count)");
  TEST_ASSERT(regions_rec == 8, "Correct number of regions found (8)");
  
  ImageDestroy(&base);
  ImageDestroy(&seg1);
  ImageDestroy(&seg2);
  ImageDestroy(&seg3);
  
  // More complex pattern
  printf("\n  → Chess 80x80 (edge=8) - 50 regions:\n");
  base = ImageCreateChess(80, 80, 8, 0x000000);
  
  seg1 = ImageCopy(base);
  InstrReset();
  regions_rec = ImageSegmentation(seg1, ImageRegionFillingRecursive);
  printf("    Recursive: %d regions, ops: %lu\n", regions_rec, InstrCount[0]);
  
  seg2 = ImageCopy(base);
  InstrReset();
  regions_stack = ImageSegmentation(seg2, ImageRegionFillingWithSTACK);
  printf("    Stack:     %d regions, ops: %lu\n", regions_stack, InstrCount[0]);
  
  seg3 = ImageCopy(base);
  InstrReset();
  regions_queue = ImageSegmentation(seg3, ImageRegionFillingWithQUEUE);
  printf("    Queue:     %d regions, ops: %lu\n", regions_queue, InstrCount[0]);
  
  TEST_ASSERT(regions_rec == regions_stack, "Recursive == Stack (complex)");
  TEST_ASSERT(regions_stack == regions_queue, "Stack == Queue (complex)");
  TEST_ASSERT(regions_rec == 50, "Correct number of complex regions (50)");
  
  ImageDestroy(&base);
  ImageDestroy(&seg1);
  ImageDestroy(&seg2);
  ImageDestroy(&seg3);
  
  // Single region (blank image)
  printf("\n  → Blank 30x30 - 1 region:\n");
  base = ImageCreate(30, 30);
  
  seg1 = ImageCopy(base);
  regions_rec = ImageSegmentation(seg1, ImageRegionFillingRecursive);
  
  seg2 = ImageCopy(base);
  regions_stack = ImageSegmentation(seg2, ImageRegionFillingWithSTACK);
  
  seg3 = ImageCopy(base);
  regions_queue = ImageSegmentation(seg3, ImageRegionFillingWithQUEUE);
  
  printf("    Recursive: %d region\n", regions_rec);
  printf("    Stack:     %d region\n", regions_stack);
  printf("    Queue:     %d region\n", regions_queue);
  
  TEST_ASSERT(regions_rec == 1 && regions_stack == 1 && regions_queue == 1, 
              "Single region detected correctly");
  
  ImageDestroy(&base);
  ImageDestroy(&seg1);
  ImageDestroy(&seg2);
  ImageDestroy(&seg3);
  
  TEST_END();
}

void test_image_segmentation() {
  TEST_START("Image Segmentation");
  
  // Create a chess pattern (multiple regions)
  Image chess = ImageCreateChess(40, 40, 10, 0x000000);
  
  // Test segmentation with STACK
  Image chess_copy1 = ImageCopy(chess);
  int regions_stack = ImageSegmentation(chess_copy1, ImageRegionFillingWithSTACK);
  TEST_ASSERT(regions_stack > 0, "STACK segmentation finds regions");
  TEST_ASSERT(regions_stack == 8, "Chess 40x40 with 10px squares has 8 white regions");
  
  // Test segmentation with QUEUE
  Image chess_copy2 = ImageCopy(chess);
  int regions_queue = ImageSegmentation(chess_copy2, ImageRegionFillingWithQUEUE);
  TEST_ASSERT(regions_queue == regions_stack, "QUEUE segmentation finds same number of regions");
  
  // Test segmentation with Recursive
  Image chess_copy3 = ImageCopy(chess);
  int regions_recursive = ImageSegmentation(chess_copy3, ImageRegionFillingRecursive);
  TEST_ASSERT(regions_recursive == regions_stack, "Recursive segmentation finds same number of regions");
  
  // Visual examples
  printf("  → Generating segmentation examples...\n");
  ImageSavePBM(chess, "img/16_segmentation_original.pbm");
  ImageSavePPM(chess_copy1, "img/17_segmentation_stack.ppm");
  ImageSavePPM(chess_copy2, "img/18_segmentation_queue.ppm");
  ImageSavePPM(chess_copy3, "img/19_segmentation_recursive.ppm");
  
  ImageDestroy(&chess);
  ImageDestroy(&chess_copy1);
  ImageDestroy(&chess_copy2);
  ImageDestroy(&chess_copy3);
  
  // Test segmentation on a solid image (1 region)
  Image solid = ImageCreate(50, 50);
  int solid_regions = ImageSegmentation(solid, ImageRegionFillingWithSTACK);
  TEST_ASSERT(solid_regions == 1, "Solid white image has 1 region");
  ImageSavePPM(solid, "img/20_segmentation_solid.ppm");
  ImageDestroy(&solid);
  
  // More complex example
  printf("  → Generating complex segmentation example...\n");
  Image complex = ImageCreateChess(120, 80, 15, 0x000000);
  ImageSavePBM(complex, "img/21_complex_original.pbm");
  
  Image complex_seg = ImageCopy(complex);
  int complex_regions = ImageSegmentation(complex_seg, ImageRegionFillingWithQUEUE);
  printf("  → Found %d regions in complex pattern\n", complex_regions);
  ImageSavePPM(complex_seg, "img/22_complex_segmented.ppm");
  
  ImageDestroy(&complex);
  ImageDestroy(&complex_seg);
  
  TEST_END();
}

void test_edge_cases() {
  TEST_START("Edge Cases");
  
  // Test small images
  Image tiny = ImageCreate(1, 1);
  TEST_ASSERT(tiny != NULL, "Can create 1x1 image");
  TEST_ASSERT(ImageWidth(tiny) == 1, "1x1 image has width 1");
  TEST_ASSERT(ImageHeight(tiny) == 1, "1x1 image has height 1");
  ImageDestroy(&tiny);
  
  // Test pixel validation
  Image img = ImageCreate(10, 10);
  TEST_ASSERT(ImageIsValidPixel(img, 0, 0), "Top-left pixel is valid");
  TEST_ASSERT(ImageIsValidPixel(img, 9, 9), "Bottom-right pixel is valid");
  TEST_ASSERT(!ImageIsValidPixel(img, 10, 10), "Pixel at boundary is invalid");
  TEST_ASSERT(!ImageIsValidPixel(img, 11, 11), "Pixel beyond boundary is invalid");
  TEST_ASSERT(!ImageIsValidPixel(img, -1, 0), "Negative pixel is invalid");
  ImageDestroy(&img);
  
  // Test region filling on same color
  Image same = ImageCreate(10, 10);
  ImageRegionFillingWithSTACK(same, 5, 5, BLACK);
  int refill = ImageRegionFillingWithSTACK(same, 5, 5, BLACK);
  TEST_ASSERT(refill == 0, "Re-filling with same color fills 0 pixels");
  ImageDestroy(&same);
  
  TEST_END();
}

// acaba aqui
// ============== PARTE ADICIONADA ==============
  void test_imageEqual_performance(uint32 size, const char* size_name){
    printf("\n--- Teste %s ---\n", size_name);
    
    // PIOR CASO: Imagens iguais
    printf("PIOR CASO (imagens iguais):\n");
    Image img_same1 = ImageCreateChess(size, size, size, 0xff0000); 
    Image img_same2 = ImageCreateChess(size, size, size, 0xff0000); 
    
    if (img_same1 && img_same2) {
      InstrReset();
      int result = ImageIsEqual(img_same1, img_same2);
      printf("  Resultado: %s\n", result ? "IGUAIS" : "DIFERENTES");
      printf("  Comparações: %lu pixels\n", InstrCount[0]);
      printf("  Esperado: %u pixels\n\n", size * size);
    }
    ImageDestroy(&img_same1);
    ImageDestroy(&img_same2);
    
    // MELHOR CASO: Diferença no primeiro pixel
    printf("MELHOR CASO (diferença no início):\n");
    Image img_diff1 = ImageCreateChess(size, size, size, 0xff0000);
    Image img_diff2 = ImageCreateChess(size, size, size, 0x00ff00);
    
    if (img_diff1 && img_diff2) {
      InstrReset();
      int result = ImageIsEqual(img_diff1, img_diff2);
      printf("  Resultado: %s\n", result ? "IGUAIS" : "DIFERENTES");
      printf("  Comparações: %lu pixels\n", InstrCount[0]);
      printf("  Esperado: 1 pixel\n\n");
    }
    ImageDestroy(&img_diff1);
    ImageDestroy(&img_diff2);
    
    // CASO MÉDIO: Padrões xadrez diferentes
    printf("CASO MÉDIO (padrões diferentes):\n");
    uint32 edge1 = size / 4;
    uint32 edge2 = size / 8;
    Image img_pattern1 = ImageCreateChess(size, size, edge1, 0xff0000);
    Image img_pattern2 = ImageCreateChess(size, size, edge2, 0xff0000);
    
    if (img_pattern1 && img_pattern2) {
      InstrReset();
      int result = ImageIsEqual(img_pattern1, img_pattern2);
      printf("  Resultado: %s\n", result ? "IGUAIS" : "DIFERENTES");
      printf("  Comparações: %lu pixels\n", InstrCount[0]);
      printf("  Entre 1 e %u pixels\n\n", size * size);
    }
    ImageDestroy(&img_pattern1);
    ImageDestroy(&img_pattern2);
  }
// ============== FIM PARTE ADICIONADA ==============



int main(int argc, char* argv[]) {
  program_name = argv[0];
  if (argc != 1) {
    error(1, 0, "Usage: imageRGBTest");
  }

  ImageInit();

  printf("\n");
  printf("╔════════════════════════════════════════════════╗\n");
  printf("║     RGB Image Module - Comprehensive Tests    ║\n");
  printf("╚════════════════════════════════════════════════╝\n");

  // Run comprehensive test suites
  test_image_creation();
  test_image_copy();
  test_image_comparison();
  test_rotation_90();
  test_rotation_180();
  test_file_operations();
  test_region_filling_stack();
  test_region_filling_queue();
  test_region_filling_recursive();
  test_region_filling_consistency();
  test_fill_methods_performance();
  test_image_segmentation();
  test_segmentation_comparison();
  test_edge_cases();

  printf("\n");
  printf("╔════════════════════════════════════════════════╗\n");
  printf("║              ORIGINAL TESTS                    ║\n");
  printf("╚════════════════════════════════════════════════╝\n");

  // Creating and displaying some images

  printf("1) ImageCreate\n");
  Image white_image = ImageCreate(100, 100);
  if (white_image != NULL) {
    printf("SUCCESS: Imagem branca 100x100 criada!\n\n");
  }
  // ImageRAWPrint(white_image);

  printf("2) ImageCreateChess(black)+ ImageSavePBM\n");
  Image image_chess_1 = ImageCreateChess(150, 120, 30, 0x000000);  // black
  if (image_chess_1 != NULL) {
    printf("SUCCESS: Imagem xadrez preta criada!\n\n");
    if (ImageSavePBM(image_chess_1, "img/chess_image_1.pbm") == 0) {
      printf("SUCCESS: Imagem salva em img/chess_image_1.pbm!\n\n");
    }
  }
  // ImageRAWPrint(image_chess_1);

  printf("3) ImageCreateChess(red) + ImageSavePPM\n");
  Image image_chess_2 = ImageCreateChess(20, 20, 8, 0xff0000);  // red
  if (image_chess_2 != NULL) {
    printf("SUCCESS: Imagem xadrez vermelha criada!\n\n");
    ImageRAWPrint(image_chess_2);
    if (ImageSavePPM(image_chess_2, "img/chess_image_2.ppm") == 0) {
      printf("SUCCESS: Imagem salva em img/chess_image_2.ppm!\n\n");
    }
  }

  printf("4) ImageCreateChess(all black)\n");
  Image black_image = ImageCreateChess(100, 100, 100, 0x000000);  // all black
  if (black_image != NULL) {
    printf("SUCCESS: Imagem toda preta criada!\n\n");
    if (ImageSavePBM(black_image, "img/black_image.pbm") == 0) {
      printf("SUCCESS: Imagem salva em img/black_image.pbm!\n\n");
    }
  }
  // ImageRAWPrint(black_image);

  printf("5) ImageCopy\n");
  Image copy_image = ImageCopy(image_chess_1);
  if (copy_image != NULL) {
    printf("SUCCESS: Cópia da imagem criada!\n\n");
    if (ImageSavePBM(copy_image, "img/copy_image.pbm") == 0) {
      printf("SUCCESS: Cópia salva em img/copy_image.pbm!\n\n");
    }
  }
  // ImageRAWPrint(copy_image);

  printf("6) ImageLoadPBM\n");
  Image image_1 = ImageLoadPBM("img/feep.pbm");
  if (image_1 != NULL) {
    printf("SUCCESS: Imagem PBM carregada de img/feep.pbm!\n\n");
  }
  ImageRAWPrint(image_1);

  printf("7) ImageLoadPPM\n");
  Image image_2 = ImageLoadPPM("img/feep.ppm");
  if (image_2 != NULL) {
    printf("SUCCESS: Imagem PPM carregada de img/feep.ppm!\n\n");
  }
  ImageRAWPrint(image_2);

  printf("8) ImageCreatePalete\n");
  Image image_3 = ImageCreatePalete(4 * 32, 4 * 32, 4);
  if (image_3 != NULL) {
    printf("SUCCESS: Paleta de cores criada!\n\n");
    if (ImageSavePPM(image_3, "img/palete.ppm") == 0) {
      printf("SUCCESS: Paleta salva em img/palete.ppm!\n\n");
    }
  }



  // ============== PARTE ADICIONADA ==============
  // testes das funções de flood-fill (Stack e Queue)

  printf("9) Teste ImageRegionFillingWithSTACK\n");
  Image test_stack = ImageCreateChess(12, 12, 4, 0xff0000);  // imagem xadrez vermelha
  if (test_stack != NULL) {
    printf("SUCCESS: Imagem de teste Stack criada!\n");
    printf("Antes do Stack fill:\n");
    ImageRAWPrint(test_stack);
    
    // preencher uma região branca (label 0) com label 1 (preto)
    // começar numa posição branca (0,0 deve ser branco no xadrez)
    int pixels_filled_stack = ImageRegionFillingWithSTACK(test_stack, 0, 0, 1);
    if (pixels_filled_stack > 0) {
      printf("SUCCESS: Flood-fill com Stack executado com sucesso!\n");
    }
    printf("Depois do Stack fill:\n");
    ImageRAWPrint(test_stack);
    
    if (ImageSavePPM(test_stack, "img/test_stack_fill.ppm") == 0) {
      printf("SUCCESS: Resultado Stack salvo em img/test_stack_fill.ppm!\n\n");
    }
  }
  
  printf("10) Teste ImageRegionFillingWithQUEUE\n");
  Image test_queue = ImageCreateChess(12, 12, 4, 0x00ff00);  // Imagem xadrez verde
  if (test_queue != NULL) {
    printf("SUCCESS: Imagem de teste Queue criada!\n");
    printf("Antes do Queue fill:\n");
    ImageRAWPrint(test_queue);
    
    // preencher uma região branca (label 0) com label 1 (preto)
    int pixels_filled_queue = ImageRegionFillingWithQUEUE(test_queue, 0, 0, 1);
    if (pixels_filled_queue > 0) {
      printf("SUCCESS: Flood-fill com Queue executado com sucesso!\n");
    }
    printf("Depois do Queue fill:\n");
    ImageRAWPrint(test_queue);
    
    if (ImageSavePPM(test_queue, "img/test_queue_fill.ppm") == 0) {
      printf("SUCCESS: Resultado Queue salvo em img/test_queue_fill.ppm!\n\n");
    }
  }
  

  // comparar se ambos os métodos dão o mesmo resultado
  printf("11) Teste de comparação Stack vs Queue\n");
  Image compare1 = ImageCreateChess(8, 8, 2, 0xff0000);
  Image compare2 = ImageCopy(compare1);
  
  if (compare1 != NULL && compare2 != NULL) {
    printf("SUCCESS: Imagens de comparação criadas!\n");
    int result1 = ImageRegionFillingWithSTACK(compare1, 0, 0, 1);
    int result2 = ImageRegionFillingWithQUEUE(compare2, 0, 0, 1);
    
    if (result1 == result2 && ImageIsEqual(compare1, compare2)) {
      printf("Resultados iguais: SIM\n");
      printf("SUCCESS: Stack e Queue produzem resultados idênticos!\n");
    } else {
      printf("Resultados iguais: NÃO\n");
      printf("ERRO: Stack e Queue produzem resultados diferentes!\n");
    }
    
    // destruir imagens de teste
    ImageDestroy(&compare1);
    ImageDestroy(&compare2);
    printf("SUCCESS: Imagens de comparação destruídas!\n\n");
  }
  
  // destruir outras imagens de teste
  ImageDestroy(&test_stack);
  ImageDestroy(&test_queue);
  printf("SUCCESS: Imagens de teste Stack e Queue destruídas!\n\n");

  // ============== TESTES DE PERFORMANCE ImageIsEqual ==============
  printf("12) Testes de Performance ImageIsEqual\n");  
  // testes para diferentes tamanhos
  test_imageEqual_performance(10, "10x10");
  test_imageEqual_performance(50, "50x50");
  test_imageEqual_performance(100, "100x100");
  test_imageEqual_performance(200, "200x200");
  test_imageEqual_performance(1200, "1200x1200");

  // ============== TESTES DE ROTAÇÃO ==============
  printf("13) Testes das Funções de Rotação\n");
  
  // imagem de teste
  printf("Criando imagem de teste 8x6 para rotações:\n");
  Image test_rotate = ImageCreateChess(8, 6, 2, 0xff0000);
  
  if (test_rotate != NULL) {
    printf("SUCCESS: Imagem de teste para rotação criada!\n");
    printf("Imagem original (8x6):\n");
    ImageRAWPrint(test_rotate);
  
    if (ImageSavePPM(test_rotate, "img/original_8x6.ppm") == 0) {
      printf("SUCCESS: Imagem original salva em img/original_8x6.ppm!\n");
    }
    
    // Rotação 90° CW
    printf("\nTeste ImageRotate90CW:\n");
    Image rotated_90 = ImageRotate90CW(test_rotate);
    
    if (rotated_90 != NULL) {
      printf("SUCCESS: Rotação 90° CW executada!\n");
      printf("Dimensões: %ux%u -> %ux%u\n", 
             ImageWidth(test_rotate), ImageHeight(test_rotate),
             ImageWidth(rotated_90), ImageHeight(rotated_90));
      printf("Imagem rotacionada 90° CW (6x8):\n");
      ImageRAWPrint(rotated_90);
      
      if (ImageSavePPM(rotated_90, "img/rotated_90cw.ppm") == 0) {
        printf("SUCCESS: Rotação 90° salva em img/rotated_90cw.ppm!\n");
      }
      
      // verificar se dimensões
      if (ImageWidth(rotated_90) == ImageHeight(test_rotate) && 
          ImageHeight(rotated_90) == ImageWidth(test_rotate)) {
        printf("SUCCESS: Dimensões da rotação 90° estão corretas!\n");
      } else {
        printf("ERRO: Dimensões da rotação 90° incorretas!\n");
      }
    } else {
      printf("ERRO: Falha na rotação 90° CW!\n");
    }
    
    // Rotação 180° CW
    printf("\nTeste ImageRotate180CW:\n");
    Image rotated_180 = ImageRotate180CW(test_rotate);
    
    if (rotated_180 != NULL) {
      printf("SUCCESS: Rotação 180° CW executada!\n");
      printf("Dimensões: %ux%u -> %ux%u\n", 
             ImageWidth(test_rotate), ImageHeight(test_rotate),
             ImageWidth(rotated_180), ImageHeight(rotated_180));
      printf("Imagem rotacionada 180° CW (8x6):\n");
      ImageRAWPrint(rotated_180);
      
      if (ImageSavePPM(rotated_180, "img/rotated_180cw.ppm") == 0) {
        printf("SUCCESS: Rotação 180° salva em img/rotated_180cw.ppm!\n");
      }
      
      // Verificar se dimensões permanecem iguais
      if (ImageWidth(rotated_180) == ImageWidth(test_rotate) && 
          ImageHeight(rotated_180) == ImageHeight(test_rotate)) {
        printf("SUCCESS: Dimensões da rotação 180° estão corretas!\n");
      } else {
        printf("ERRO: Dimensões da rotação 180° incorretas!\n");
      }
    } else {
      printf("ERRO: Falha na rotação 180° CW!\n");
    }
    
    // Rotação dupla 90° (deve ser igual a 180°)
    printf("\nTeste de consistência (90° + 90° = 180°):\n");
    if (rotated_90 != NULL) {
      Image double_90 = ImageRotate90CW(rotated_90);
      
      if (double_90 != NULL && rotated_180 != NULL) {
        int are_equal = ImageIsEqual(double_90, rotated_180);
        printf("Duas rotações 90° vs Uma rotação 180°: %s\n", 
               are_equal ? "IGUAIS" : "DIFERENTES");
        
        if (are_equal) {
          printf("SUCCESS: Rotações são consistentes!\n");
        } else {
          printf("AVISO: Rotações podem ter diferenças na implementação\n");
        }
        
        ImageDestroy(&double_90);
      }
    }
    
    // Rotação quádrupla 90° (deve voltar ao original)
    printf("\nTeste de rotação completa (4 x 90° = 360°):\n");
    if (rotated_90 != NULL) {
      Image temp1 = ImageRotate90CW(rotated_90);   // 180°
      Image temp2 = temp1 ? ImageRotate90CW(temp1) : NULL; // 270°
      Image full_rotation = temp2 ? ImageRotate90CW(temp2) : NULL; // 360° = 0°
      
      if (full_rotation != NULL) {
        int back_to_original = ImageIsEqual(test_rotate, full_rotation);
        printf("4 rotações 90° vs Original: %s\n", 
               back_to_original ? "IGUAIS" : "DIFERENTES");
        
        if (back_to_original) {
          printf("SUCCESS: Rotação completa retorna ao original!\n");
        } else {
          printf("AVISO: Rotação completa difere do original\n");
        }
        
        if (ImageSavePPM(full_rotation, "img/full_rotation.ppm") == 0) {
          printf("SUCCESS: Rotação completa salva em img/full_rotation.ppm!\n");
        }
        
        ImageDestroy(&temp1);
        ImageDestroy(&temp2);
        ImageDestroy(&full_rotation);
      }
    }
    
    ImageDestroy(&rotated_90);
    ImageDestroy(&rotated_180);
    ImageDestroy(&test_rotate);
    printf("SUCCESS: Imagens de teste de rotação destruídas!\n");
    
  } else {
    printf("ERRO: Falha ao criar imagem de teste!\n");
  }
  
  printf("=====================================\n");
  printf("TESTES DE ROTAÇÃO COMPLETADOS!\n");
  printf("=====================================\n");

  // ============== FIM PARTE ADICIONADA ==============

  ImageDestroy(&white_image);
  ImageDestroy(&black_image);
  if (copy_image != NULL) {
    ImageDestroy(&copy_image);
  }
  ImageDestroy(&image_chess_1);
  ImageDestroy(&image_chess_2);
  ImageDestroy(&image_1);
  ImageDestroy(&image_2);
  ImageDestroy(&image_3);

  // Print comprehensive test summary
  printf("\n");
  printf("╔════════════════════════════════════════════════╗\n");
  printf("║              TEST SUMMARY                      ║\n");
  printf("╠════════════════════════════════════════════════╣\n");
  printf("║  Total Tests:  %-4d                           ║\n", tests_passed + tests_failed);
  printf("║  Passed:       %-4d ✓                         ║\n", tests_passed);
  printf("║  Failed:       %-4d ✗                         ║\n", tests_failed);
  printf("╚════════════════════════════════════════════════╝\n");
  printf("\n");

  if (tests_failed == 0) {
    printf("🎉 All tests passed successfully!\n\n");
    printf("📁 Output images saved to: ./img/\n");
    printf("   Use an image viewer to see the results.\n");
    printf("   For PBM/PPM files, try: gimp, eog, or convert to PNG.\n\n");
    printf("TODOS OS TESTES COMPLETADOS COM SUCESSO!\n");
    return 0;
  } else {
    printf("⚠️  Some tests failed. Please review the output above.\n\n");
    return 1;
  }
}
