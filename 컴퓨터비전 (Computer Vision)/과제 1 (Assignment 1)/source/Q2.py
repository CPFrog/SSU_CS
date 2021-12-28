import numpy as np
import cv2 as cv

# saturation을 처리하는 함수.
def saturate(value):
    if value+50>255:
        return 255 # 더했을 때 밝기가 255 넘어가면 255로 saturate
    else: 
        return value+50 # 아닐 경우 더한 값을 return

def partialBrighter2():
    src = cv.imread('lenna.bmp', cv.IMREAD_GRAYSCALE) # 회색조로 이미지 불러오기 위한 부분

    # 파일을 열지 못했을 경우 오류 문구 출력 후 종료
    if src is None:
        print('지정한 파일을 찾을 수 없습니다.\n')
        return

    dst=src.copy() # 원본 이미지를 dst에 복사.

    # 해결방법 1 : 반복문을 이용한 부분 밝기 변경 및 saturation 처리.
    for y in range(200, 401):
        for x in range(200, 401):
            dst[y,x]=saturate(dst[y,x])

    # 해결방법 2 : OpenCV에서 제공하는 add 함수를 이용해 밝기 변화 및 saturation을 한번에 처리.
    # dst[200:400,200:400]=cv.add(dst[200:400, 200:400], 50)
  
    cv.imwrite('lenna_50.bmp', dst) # 'lenna_50.bmp' 라는 이름으로 파일을 저장.

partialBrighter2()