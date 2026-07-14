# Cube Dog!

## プレイ動画
[https://youtu.be/fP4A_1HFYFs](https://youtu.be/fP4A_1HFYFs)

## パッケージ
[https://carnivals.itch.io/cubedog](https://carnivals.itch.io/cubedog)

## 1. プロジェクト概要
本作はUnrealEngine5.5を使用して制作した、3Dパズルアクションゲームです。<br>
箱を生成してステージを攻略するのが目的になります。

## 2. 開発環境
- **開発期間**: 2026年4月～2026年6月（約3か月）
- **言語**: C++
- **エンジン**: Unreal Engine 5.5
- **プラットフォーム**: Windows

## 3. 基本的な操作方法
- **移動**: WASD / 左スティック
- **ジャンプ**: Space / Aボタン
- **箱の生成**: 左クリック / Xボタン
- **カメラ操作**: マウス / 右スティック
- **ポーズ**: P / Optionボタン
- **デバッグメニュー**: Ctrl + G

## 4. 工夫した点
- 基本的な実装はすべてC++で行い、エフェクトや効果音の再生部分をBPに逃がす設計にしました。
- 箱の生成はオブジェクトプールを用いて管理するようにしました。
	- 10個以上箱を生成すると古いものが消えるようにしています。
	- その際、次に消える箱は赤く点滅するようにしました。
	- また、プレイヤーが乗っている箱は消えないようにしました。
	- [CubePetsCharacter.h](Source/CubePets/Public/Characters/CubePetsCharacter.h)
	- [CubePetsCharacter.cpp](Source/CubePets/Private/Characters/CubePetsCharacter.cpp)
- UI画面はすべてEnhancedInputを採用して自作することで標準機能に捕らわれない実装を実現しました。
	- [CubePetsTitlePlayerController.h](Source/CubePets/Public/System/Title/CubePetsTitlePlayerController.h)
	- [CubePetsTitlePlayerController.cpp](Source/CubePets/Private/System/Title/CubePetsTitlePlayerController.cpp)
	- [CubePetsSelectPlayerController.h](Source/CubePets/Public/System/StageSelect/CubePetsSelectPlayerController.h)
	- [CubePetsSelectPlayerController.cpp](Source/CubePets/Private/System/StageSelect/CubePetsSelectPlayerController.cpp)
- テキストはCSVから読み込むようにすることで効率化し、ローカライズにも対応しました。
	- [CSVTextBlock.h](Source/CubePets/Public/UI/CSVTextBlock.h)
	- [CSVTextBlock.cpp](Source/CubePets/Private/UI/CSVTextBlock.cpp)
- ボタンアイコンが入力デバイスによって変化するようにしました。Subsystemに共通化することで使いやすくしています。
	- [CubePetsInputDeviceSubsystem.h](Source/CubePets/Public/Subsystems/CubePetsInputDeviceSubsystem.h)
	- [CubePetsInputDeviceSubsystem.cpp](Source/CubePets/Private/Subsystems/CubePetsInputDeviceSubsystem.cpp)
- 実務を見据えてImGuiを用いたデバッグ機能を実装しました。
	- [DebugMenuSubsystem.h](Source/CubePets/Public/Subsystems/DebugMenuSubsystem.h)
	- [DebugMenuSubsystem.cpp](Source/CubePets/Private/Subsystems/DebugMenuSubsystem.cpp) 
- 各ステージに隠しメダルと最小手数ミッションを実装することでゲームとしてのやりこみ要素を組み込みました。
	- [GameProgressionSubsystem.h](Source/CubePets/Public/Subsystems/GameProgressionSubsystem.h)
	- [GameProgressionSubsystem.cpp](Source/CubePets/Private/Subsystems/GameProgressionSubsystem.cpp)
- ギミックにはGASを採用しました。これにより例えば「大砲が、撃った弾が消えるまで次の弾は撃たない」といった処理がタグで簡単に管理できました。（GameAbilityはBPで実装しております）
	- [GimmickBase.h](Source/CubePets/Public/Gimmicks/GimmickBase.h)
	- [GimmickBase.cpp](Source/CubePets/Private/Gimmicks/GimmickBase.cpp)

## 5. 今後の課題、展望
- 箱はプールしているが、大砲の弾などは毎回スポーンしているので改善の余地がありそうです。
- すべてのPlayerControllerでボタンアイコン切り替え用の関数InputKeyを重複して書いているためPlayerControllerBaseのような基底クラスに書くべきでした。
- ギミックのOverlap処理の導線を、個別に毎回バインド含めて重複して書いていたが、これも基底クラスに処理を共通化できたように思います。
- ショップ機能の実装をしたいです。（展望）
   - メダルという要素を拡張し、集めたメダルとキャラクターを交換、入手したキャラはゲームで使用できるといった機能を想定しています。

## 6. 概要書
[Cube Dog_概要書](https://fair-ground-371.notion.site/CubePets-37b27004c21780d0a9bdda819613f3b4)

