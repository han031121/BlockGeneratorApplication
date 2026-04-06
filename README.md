# BlockGeneratorApplication

## 개요

사용자가 지정한 조건에 맞게 쌓기나무 블록 군집을 무작위로 생성하고,
이를 2D 이미지로 렌더링하여 저장할 수 있는 프로그램입니다.

<img src = "https://github.com/han031121/BlockGeneratorApplication/blob/master/example_image.png" width = "800" height = "800">

## 실행 환경

- Windows 10/11 64-bit

## 설치

### [1.0.1 Release](https://github.com/han031121/BlockGeneratorApplication/releases/tag/v1.0.1)

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

- **Draw Settings** : 이미지에 관련된 내용을 정합니다.

    - **Cam degree** : 카메라의 각도를 정합니다.

    - **Light degree** : 광원의 각도를 정합니다.

    - **Line thickness** : 블록 테두리의 두께를 정합니다.

    - **Magnification** : 이미지 상의 블록의 크기를 늘리거나 줄입니다.

    - **Fix light degree** : 카메라의 수평 각도와 광원의 수평 각도를 같게 
    합니다.

    - **Draw color** : 블록의 색상을 정합니다.

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

## 주의사항

- 중복 생성 방지 옵션을 선택한 경우, 크기가 큰 군집에 대해서 후반부에 성능 저하가 우려됩니다.

- 중복 생성 방지 옵션을 선택한 경우, 아직 생성되지 않은 모양이 존재함에도 불구하고 생성이 중단될 가능성이 있습니다.

- 생성하는 이미지의 해상도는 고정되어 있습니다. (1000 x 1000)

- 앞의 블록에 가려져 높이를 알 수 없는 부분이 존재할 수 있습니다.

---

## 개발

이미지 생성 및 GUI는 **[openFrameworks](https://openframeworks.cc/)** 를 이용해 만들어졌습니다.

### 개발 환경
- OS: Windows 11
- openFrameworks 버전: 0.12.1
- IDE: Visual Studio 2022
- C++ 표준: C++17

### 주요 클래스 

- **blockData** : 블록의 데이터를 생성하고 관리합니다.

- **drawObject** : 블록 데이터를 기반으로 이미지를 렌더링합니다.

- **ofApp** : 프로그램의 입출력, 상태 갱신, GUI를 담당합니다.

### 기타

- 설정 파일(json)은 `data` 폴더에 저장됩니다.
- `data` 폴더의 폰트 파일이 없으면 실행되지 않습니다.
- 이미지는 `saved_images` 폴더에 저장됩니다.

## 알고리즘

### 블록 생성 절차
1. `block_count_1`과 `block_count_2` 사이의 범위에서 하나의 값을 `block_count`로 정합니다.
2. `density` 값에 따른 가중치를 최대 크기 내의 모든 좌표에 할당합니다.
3. 블록 생성 시작 위치를 정하고, 블록 하나를 생성합니다. 이때, 시작 위치는 가중치의 영향을 받습니다.
4. 모든 생성된 블록들에 대해 인접한 좌표를 생성 위치 후보로 설정합니다. 생성 위치 후보들 중 하나를 가중치 기반 무작위 선택으로 결정한 뒤, 해당 위치에 블록을 생성합니다.
5. 생성된 블록의 개수가 `block_count`만큼 될 때 까지 4번 과정을 반복합니다.

### 중복 제거
1. 블록이 생성되면 unordered_set에 생성된 블록 군집의 고윳값을 저장합니다. 
2. 블록 군집 생성 시 unordered_set에 고윳값이 이미 저장되어 있다면 다시 생성합니다.
3. 일정 횟수 이상 생성을 시도했다면 더 이상 블록을 생성할 수 없는 것으로 판단하고, 블록 생성이 중단됩니다.

### 가려진 블록 생성 방지
생성 위치 후보를 설정하는 과정에서, 현재 위치가 생성 가능한 위치인지 판별하는 과정을 거칩니다. 이때, 현재 위치에 블록이 생성되면, 인접한 블록 중 완전히 가려지는 블록이 있는지 판별합니다.

인접한 블록에 대해서만 판별하기 때문에, 블록 군집의 규모가 커질수록 정확도가 떨어집니다.

## 추후 계획

- GUI 개선
- 가려진 블록 생성 방지 알고리즘 개선
