#include <cstdio>
#include <cstdlib>
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
  int top, bottom, left, right;
  top = img.rows;
  bottom = img.rows;
  left = img.cols;
  right = img.cols;

  int type = cv::BORDER_REFLECT101;//BORDER_CONSTANTでゼロパディング、BORDER_WRAPで周期拡張、BORDER_REFLECT101で対象周期拡張(奇数)

  cv::copyMakeBorder(img, dst, top, bottom, left, right, type);


  // 画像の表示
  cv::imshow("Original", img);
  cv::imshow("extended", dst);
  // キー入力を待つ
  cv::waitKey();
  // 全てのウインドウを破棄
  cv::destroyAllWindows();

  return EXIT_SUCCESS;
}
