# Block Generator

## 개요

사용자가 지정한 조건에 맞게 쌓기나무 블록 군집을 무작위로 생성하고,
이를 2D 이미지로 렌더링하여 저장할 수 있는 프로그램입니다.

<img src = "https://github.com/han031121/BlockGeneratorApplication/blob/master/example_image.png" width = "800" height = "800">

## 실행 환경

- Windows 10/11 64-bit

## 설치

### [1.0.3 Release](https://github.com/han031121/Block-Generator/releases/tag/v1.0.3)

위 링크에서 **block_generator.zip**을 다운로드 및 압축 해제 후, **blockGeneratorApplication.exe**을 실행해 주세요.

실행 시 필요한 파일이 누락되어 정상 실행이 불가한 경우, zip파일에 포함된 VC_redist.x64.exe를 먼저 실행해 주세요.

## 사용 방법

1. 가장 왼쪽의 **BLOCK MENU**에서 생성하고자 하는 블록의 조건을 설정합니다.
2. **Set block** 버튼을 클릭하여 설정을 완료합니다.
3. **Generate block** 버튼을 클릭하면 조건에 맞는 블록 군집을 생성합니다.
4. 오른쪽의 **DRAW MENU**에서 블록 이미지 설정을 변경하거나 및 이미지를 저장할 수 있습니다.

---

## 상세 설명

### BLOCK MENU

- **Block Settings** : 생성할 블록 군집의 조건을 설정합니다.

    - **Min block count** / **Max block count** : 생성될 블록 군집의 최소/최대 블록 개수를 정합니다.

    - **Max row** / **Max column** / **Max height** : 생성될 블록 군집의 최대 크기를 정합니다. (행/열/높이)

    - **Density** : 블록 군집의 밀도를 정합니다. 밀도가 높을 수록 블록이 뭉쳐서 생성됩니다.

    - **Allow duplication** : 현재 설정에 대해 중복되는 모양의 블록 군집의 생성 허가 여부를 정합니다.

- **Current Settings** : 현재 설정을 출력합니다.

- **Current Block Info** : 현재 블록 군집의 정보를 출력합니다.

- **Block generation** : 블록 군집 생성에 대한 동작을 합니다.

    - **Set block** : 블록 군집 조건 설정을 완료합니다.

    - **Generate block** : 조건에 맞는 블록 군집을 무작위로 생성합니다.

### DRAW MENU

- **Draw Settings** : 이미지에 관련된 내용을 설정합니다.

    - **Cam degree** : 카메라의 각도를 조절합니다.

    - **Light degree** : 광원의 각도를 조절합니다.

    - **Line thickness** : 블록 테두리의 두께를 조절합니다.

    - **Magnification** : 카메라의 배율을 조절합니다.

    - **Fix light degree** : 카메라의 수평 각도와 광원의 수평 각도를 같게 
    합니다.

    - **Draw color** : 블록의 색상을 설정합니다.

- **Draw Functions** : 이미지와 관련된 동작을 합니다.

    - **Save image** : 현재 이미지를 저장합니다. 저장 위치는 `saved_images`입니다.

    - **Reset settings** : 이미지 관련 설정을 초기화합니다.

### 키보드 조작

- **N** : Set block
- **G** : Generate block
- **S** : Save image
- **R** : Reset settings
- **-** : gui 크기 줄이기
- **=** : gui 크기 늘리기
- **Tab** : gui 토글

### 마우스 조작

- 마우스 드래그 : 카메라의 각도를 조절합니다.
- 마우스 스크롤 : 카메라의 배율을 조절합니다.

## 주의사항

- 중복 생성 방지 옵션을 선택한 경우, 아직 생성되지 않은 모양이 존재함에도 불구하고 생성이 중단될 가능성이 있습니다.

- 생성하는 이미지의 해상도는 고정되어 있습니다. (1000 x 1000)

- 앞의 블록에 가려져 높이를 알 수 없는 부분이 존재할 수 있습니다.

---

## 개발

이미지 생성 및 GUI는 **[openFrameworks](https://openframeworks.cc/)** 를 이용해 만들어졌습니다.

- OS: Windows 11
- openFrameworks 버전: 0.12.1
- IDE: Visual Studio 2022
- C++ 표준: C++17

## 추후 계획

- GUI 개선
- 가려진 블록 생성 방지 알고리즘 개선
