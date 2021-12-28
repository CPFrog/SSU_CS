import numpy as np
import cv2 as cv
import pytesseract as pyts
from copy import *

# GoodFeaturesToTrack 방식을 이용해 corner 검출
def corner_gftt(src): 
    gray=cv.cvtColor(src,cv.COLOR_BGR2GRAY)

    # edge 검출 전 이미지 사전 가공 작업
    _,src_binary=cv.threshold(gray,0,255,cv.THRESH_BINARY|cv.THRESH_OTSU) # 이미지 이진화 작업
    gftt=cv.GFTTDetector_create(minDistance=50) # gftt 검출기 생성
    op=src_binary
    op=cv.erode(op,np.ones((3,3),np.uint8),iterations=2) # 침식 2회 수행
    op=cv.dilate(op,np.ones((3,3),np.uint8),iterations=1) # 팽창 1회 수행
    keypoint1=gftt.detect(op,None) # 특징점(keypoint) 검출

    # 추출한 특징점들을 하나의 리스트에 담기 위한 작업.
    kp=[]
    for obj in keypoint1:
        x,y=obj.pt
        x=round(x)
        y=round(y)
        kp.append([x,y])
    kp.sort(key=lambda tmp:(tmp[1],tmp[0])) # 특징점들을 y 좌표 (세로축) 순서대로 정렬
    x1,y1=kp[0] # 영수증 좌상단 꼭지점
    x2,y2=kp[1] # 영수증 우상단 꼭지점
    x3,y3=kp[-1] # 영수증 우하단 꼭지점
    x4,y4=kp[-2] # 영수증 좌하단 꼭지점
    
    # 영수증 출력 윈도우 크기 설정 (원본 영상 * 1.5배 크기로)
    w=round(min(x2-x1,x3-x4)*1.5) # 너비 설정
    h=round(min(y3-y2,y4-y1)*1.5) # 높이 설정
    # print(w, h)

    # 원본 사진의 좌표를 각각 결과창의 어느 좌표에 매핑시킬건지 결정
    src_pts=np.array([[x1,y1],[x2,y2],[x3,y3],[x4,y4]]).astype(np.float32)
    dst_pts=np.array([[0,0],[w-1,0],[w-1,h-1],[0,h-1]]).astype(np.float32)

    # 위에서 정한 정책대로 투영 변환 수행
    pers_mat=cv.getPerspectiveTransform(src_pts,dst_pts)
    dst=cv.warpPerspective(src,pers_mat,(w,h))

    # print(x1, y1, x2, y2)
    # print(x4, y4, x3, y3)

    # cv.waitKey()
    # cv.destroyAllWindows()

    return dst
    
def preprocess(img):
    img_gray=cv.cvtColor(img,cv.COLOR_BGR2GRAY)

    # 양방향 필터를 이용한 노이즈 최소화, 언샤프 마스크 필터를 이용한 샤프닝 수행
    bilat=cv.bilateralFilter(img_gray,-1,10,5)
    blur=cv.GaussianBlur(bilat,(0,0),5) # 가우시안 블러 수행
    denoise=cv.addWeighted(bilat,1+1.,blur,-1.0,0.0)
    
    # 영상의 이진화 수행
    _,bin=cv.threshold(denoise,165,255,cv.THRESH_BINARY_INV)

    # 모폴로지 연산(닫기) 수행
    morp=cv.morphologyEx(bin,cv.MORPH_CLOSE,None)
    

    return morp

# Tesseract 사용 참고 링크
# 1. https://velog.io/@latte_h/Tesseract
# 2. https://www.opcito.com/blogs/extracting-text-from-images-with-tesseract-ocr-opencv-and-python
# 3. 커스텀 설정 : https://stackoverflow.com/questions/24379781/how-can-i-run-tesseract-with-multiple-languages-one-time
def ocr(image_color,image_bin):
    custom_config = r'-l eng+kor --psm 6' # 영어와 한국어 동시 사용 위한 커스텀 설정
    details=pyts.image_to_data(image_color,config=custom_config,output_type=pyts.Output.DICT,lang='kor')
    
    Img_With_Box=deepcopy(image_color) # 영수증 사진 위에 텍스트 바운딩 박스 표시하기 위한 deep copy
    
    # 영수증 사진에 바운딩 박스 표기 위한 과정
    n=len(details['text'])
    for i in range(n):
        if int(details['conf'][i]) in range(0,150): # 150 조건에 맞는 바운딩 박스만 정상적인 것으로 취급
            x=details['left'][i]
            y=details['top'][i]
            w=details['width'][i]
            h=details['height'][i]
            Img_With_Box=cv.rectangle(Img_With_Box,(x,y),(x+w,y+h),(0,0,255),2)
    
    cv.imwrite('Bounding Box.jpg',Img_With_Box)
    
    text=pyts.image_to_string(image_color, config=custom_config, lang='kor') # tesseract를 이용한 텍스트 추출

    return text

def main(): 
    src=cv.imread('Receipt.jpg')

    if src is None:
        print('이미지를 불러올 수 없습니다.')
        return
    
    receipt_color=corner_gftt(src)

    if receipt_color is None:
        print('코너 검출 과정 중 문제가 발생했습니다.')
        return
    
    morp_receipt=preprocess(receipt_color)

    receipt_text=ocr(receipt_color,morp_receipt)

    # cv.imshow('original image', src) # 원본 영상
    # cv.imshow('morphed',receipt_color) # 장방형으로 영역을 추출한 영수증 이미지 출력
    # cv.imshow('preprocessed',morp_receipt) # morphology 연산을 수행한 뒤 이미지 출력
    # print(receipt_text)

    # cv.waitKey()
    # cv.destroyAllWindows()

main()