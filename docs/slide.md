# VSTを作る

---

## ぼく
- - -
まのめっち(@Mano_tetsu)  
藤本研  
秋葉で服脱がしたり世界を旅してる  
卒研つらい

---

## 今日はついに曲の話をします！

---

## VSTってなに？
- - -
Steinberg Virtual Studio Technology  
プラグインの形をとる  
曲作るときには欠かせない  
VSTi(VST Instruments)は楽器の代わり

---

## なにができる？
- - -
音が出せる(VSTi)  
音にエフェクトがかけられる

---

## 今回のテーマ
- - -
トレモロエフェクトを作る  
(音がウワンウワンってなる)

---

## 何が必要？
- - -
VST SDK  
VC++  
音の知識

---

## 設計
- - -
ステレオ入力・ステレオ出力
SIN波  
波の振幅とHzを変えられるようにする

---

## 作る

---

## 設定
- - -
SDKはSteinberg社が出してるのをDL  
Visual Studioであれこれ設定  
(ヘッダファイルとかね)

---

## コーディング
- - -
音声信号を処理するのとかはSDKが用意してる  
クラスを呼ぶだけで結構いろいろやってくれる

---

## トレモロの処理
- - -
```cpp
  switch (index)
  {
    case TREMOLO_SPEED:
      fTremoloSpeed = 1.0f / (29.0f*value +1.0f); //1秒～1/30秒の範囲にする
      break;
    case TREMOLO_DEPTH:
      fTremoloDepth = value;
      break;
  }
```

---

## 出力
- - -

```cpp
while (sampleFrames >= 0)
{
  //ここで音声処理を行う。

  //dt分時間を進める。
  //トレモロ周期を超えた場合は、周期分戻す。
  fTime = fTime + dt;
  if (fTime > fTremoloSpeed)
  {
    fTime -= fTremoloSpeed;
  }

  //出力バッファへ書き込む。
  (*outL) = (*inL) * ( 1.0f - fTremoloDepth *
    (0.5f + 0.5f*sin(2.0f*pi*fTime/fTremoloSpeed)));
  (*outR) = (*inR) * ( 1.0f - fTremoloDepth *
    (0.5f + 0.5f*sin(2.0f*pi*fTime/fTremoloSpeed)));

  inL++;
  inR++;
  outL++;
  outR++;
  sampleFrames--;
}
```
**

---

## コンパイル
- - -
VSでビルドするだけ  
dllファイルができる(重要)

---

## 動かしてみる
- - -
いえい

---

## まとめ
- - -
C++わからないけどなんとかなった  
以外に簡単に作れる  
次はVSTiも作りたい
