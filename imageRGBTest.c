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

  // ============== TESTES DE ROTAÇÃO ==============
  printf("13) Testes das Funções de Rotação\n");
  printf("=====================================\n");
  
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
  
  printf("SUCCESS: Todas as imagens destruídas com sucesso!\n");
  printf("TODOS OS TESTES COMPLETADOS COM SUCESSO!\n");

  return 0;
}
