import numpy as np
import cv2 as cv

def partialBrighter():
    src = cv.imread('lenna.bmp', cv.IMREAD_GRAYSCALE) #회색조로 이미지 불러오기 위한 부분

    if src is None:
        print('지정한 파일을 찾을 수 없습니다.\n')
        return

    dst = src.copy() #[200:400, 200:400] 까지의 부분만을 dst에 복사

    dst[200:400, 200:400]+=20 #회색조 이미지이므로 전체 픽셀에 20만큼 밝아짐

    cv.imwrite('lenna_20.bmp',dst) #'lenna_20.bmp' 라는 이름으로 파일을 저장.

partialBrighter()