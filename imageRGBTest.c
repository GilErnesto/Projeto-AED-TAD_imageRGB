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
  
  printf("SUCCESS: Todas as imagens destruídas com sucesso!\n");
  printf("TODOS OS TESTES COMPLETADOS COM SUCESSO!\n");

  return 0;
}
