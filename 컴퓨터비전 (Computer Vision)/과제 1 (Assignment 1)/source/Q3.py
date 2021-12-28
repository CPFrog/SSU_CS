import cv2 as cv

# 원본 영상 파일을 불러옴
cap=cv.VideoCapture('stopwatch.avi')

# 영상을 불러오지 못했을 경우 오류 출력 및 종료.
if not cap.isOpened():
    print('지정한 비디오 파일을 열 수 없습니다.\n')
    exit()

fourcc = cv.VideoWriter_fourcc(*'DIVX') # 영상 압축 방식 지정 (Windows 이므로 DIVX)
fps=int(cap.get(cv.CAP_PROP_FPS)) # 영상의 프레임/초 불러옴
w=int(round(cap.get(cv.CAP_PROP_FRAME_WIDTH))) # 영상의 넓이를 불러옴
h=int(round(cap.get(cv.CAP_PROP_FRAME_HEIGHT))) # 영상의 높이를 불러옴
outputVideo = cv.VideoWriter('stopwatch_inv.avi',fourcc,fps,(w,h)) # 저장될 파일 생성

# 저장될 파일을 생성하지 못한 경우 오류 출력 및 종료.
if not outputVideo.isOpened():
    print('저장할 파일을 생성하지 못했습니다.\n')
    exit()

pos=0 # 현재 작업중인 프레임이 몇 번째 프레임인지 판단하는 변수

while True:
    ret, frame=cap.read() # 1개의 프레임을 불러옴

    if not ret: # 영상의 끝인 경우 반복문 탈출
        break

    # 영상이 10초 이전이라면 원본 영상의 프레임을 그대로 복사하여 파일에 기록
    if pos <= fps*10:
        outputVideo.write(frame) 
    
    # 영상이 10초 이후라면 원본 영상의 오른쪽 아래 (전체 영상의 1/4부분) 부분을 색상 반전 시킨 후 파일에 기록
    else:
        inversed=frame.copy()
        inversed[int(h/2):,int(w/2):]=~inversed[int(h/2):,int(w/2):] 
        outputVideo.write(inversed)

    pos+=1