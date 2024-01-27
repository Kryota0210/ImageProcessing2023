#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cmath>
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

//クリッピング処理
auto clip = [](int v){      //画素値が0~255なので、範囲を超えたらその範囲限界に定義する。(ラムダ式)
  if(v > 255){
    v = 255;
  }
  if(v < 0){
    v = 0;
  }
  return v;
};

//ガンマ補正を行うラムダ式
auto gamma_correction = [](int v, double gamma){
  double in = static_cast<double>(v) / 255.0;   //inをdoubleに定義 (cはcastは(double)だが、c++はstatic_cast<double>()になる)
  double out = pow(in , gamma) * 255.0;         //pow : #include<cmath>で使える。べき乗。
  return static_cast<int>(out);                 //int型に戻して整数で表示
};

//量子化(一様線形量子化)を行うラムダ式
auto quantization = [](int v, double delta){
  double x = static_cast<double>(v);
  double Qx = floor(x / delta);                 //floor : 切り捨て
  return Qx;
};

//逆量子化を行うラムダ式
auto dequantization = [](double v, double delta){
  double rec = v * delta;
  return static_cast<int>(rec);
};


const int W = img.cols;
const int H = img.rows;
uint8_t *pixels = img.data;
const double delta = 32.0;                //値変更で画質変更(初期値1.0)

//ストライドアクセス
  for(int y = 0; y < H; ++y){
    for(int x = 0; x < W; ++x){
      int idx = y * W + x;                //ストライドアクセスによるメモリ上のアドレスの計算
      //printf("%3d ", img.data[idx]);    //画素数の表示

      int val = pixels[idx];              //画素値の取得

      //処理
      //val += 128;                       //加算処理
      //val = gamma_correction(val, 1.5); //ガンマ補正
      val = dequantization(quantization(val, delta), delta);    //量子化(一様線形量子化)

      pixels[idx] = clip(val);            //処理結果の画素値をメモリに書き込む
    }
  }


  // 画像の表示
  cv::imshow("window", img);
  // キー入力を待つ
  cv::waitKey();
  // 全てのウインドウを破棄
  cv::destroyAllWindows();

  return EXIT_SUCCESS;
}
