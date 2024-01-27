#include <cstdio>
#include <cstdlib>
#include <memory>
#include <opencv2/opencv.hpp>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Input image is required.\n");
    return EXIT_FAILURE;
  }

  // 画像の読み込み
  cv::Mat img = cv::imread(argv[1], cv::IMREAD_ANYCOLOR);
  if (img.empty()) {  // 画像ファイルが見つからない場合の処理
    printf("Input image is not found.\n");
    return EXIT_FAILURE;
  }

  cv::Mat dst;
  int type = cv::BORDER_REFLECT101;//BORDER_CONSTANTでゼロパディング、BORDER_WRAPで周期拡張、BORDER_REFLECT101で対象周期拡張(奇数)

  const int xsize = 3;
  const int ysize = 3;
  auto coeff = std::make_unique<float []>(xsize * ysize);      //#include<memory>で使用可能, 動的にメモリ確保(newもあるがこっち推奨)

  for(int i = 0; i < xsize * ysize; ++i){
    coeff[i] = 1.0F / (xsize * ysize);
  }

  cv::Mat filter(cv::Size(xsize, ysize), CV_32FC1, coeff.get());

  cv::filter2D(img, dst, CV_8U, filter, cv::Point(-1, -1), 0.0, type);  //白黒画像はCV_8U(8bit), 

  // 画像の表示
  cv::imshow("Original", img);
  cv::imshow("extended", dst);
  // キー入力を待つ
  cv::waitKey();
  // 全てのウインドウを破棄
  cv::destroyAllWindows();

  //delete[] coeff;           //メモリの解放, unique-ptrの場合不要

  return EXIT_SUCCESS;
}
