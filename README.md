### 1. 프로젝트 소개
#### 1.1. 배경 및 필요성
인공지능(AI)과 사물인터넷(IoT) 기술의 발전으로 인해 스마트 홈 디바이스의 사용이 증가하면서 발생하는 임베디드 시스템의 보안 문제가 많이 발생하고 있다.
이러한 문제의 원인으로는 운영체제의 취약점, 인증 메커니즘의 부재, 데이터 보호 부족 등이 있다. 
민감한 데이터가 암호화되지 않을 경우 시스템 전체의 보안이 위협받을 수 있으므로, 경량화된 보안 솔루션의 필요성이 대두된다.
따라서 주로 임베디드 시스템에 사용되는 QNX 운영체제의 보안 취약점을 분석하고, 저사양 임베디드 시스템에서 사용할 수 있는 경량화된 보안 알고리즘을 개발하려고 한다.

#### 1.2. 목표 및 주요 내용
##### 1.2.1. 암호 알고리즘 경량화
기존에 있는 암호 알고리즘을 이해하고 경량화 할 수 있는 방안을 제시한다.
- 주요 내용
  - AES 암호 알고리즘
  - CTR 운영모드
  - AES 암호 알고리즘 경량화
##### 1.2.2. QNX 취약점 분석
QNX 운영체제의 보안 취약점을 식별하고 분석하는 것을 목표로 함.
- 주요 내용
  - QNX 취약점 예시
  - QNX 환경 구성
  - QNX 취약점 분석을 위한 다양한 사이버 공격


### 2. 상세설계
#### 2.1.1 암호 알고리즘 경량화 상세설계
알고리즘 설명 | 알고리즘 설명
:---:|:---:|
<img src = "https://github.com/user-attachments/assets/9b07136e-170d-4d5a-b072-8b4aca0bbe01"> | <img src = "https://github.com/user-attachments/assets/b50ac5f6-8ffb-40cc-8287-c1efecc5f6c8">
사전연산 알고리즘|바이트 수를 경량화한 알고리즘
<img src = "https://github.com/user-attachments/assets/83fda375-5d82-43af-9585-9bd754ff1322" width = "50%" height = "50%"> | <img src = "https://github.com/user-attachments/assets/284c4b0b-6e97-4a1e-b6c3-e252a40ffb4c">
ShiftRows 제거 알고리즘|MixColumns 사전연산 알고리즘
#### 2.1.2 QNX 취약점 분석
공격 시연 | 공격 시연
:---:|:---:|
<img src = "https://github.com/user-attachments/assets/aead9fbc-4037-4ed0-a5b1-3e676f8751a3" width = "50%" height = "50%" > | <img src = "https://github.com/user-attachments/assets/cc6e058a-1b03-411d-a799-81f58262aac4" width = "50%" height = "50%" >
Nmap | Ping Flooding
<img src = "https://github.com/user-attachments/assets/d2e6e6f7-ba57-4b47-a0d8-d4a0008d315a" width = "50%" height = "50%"> | <img src = "https://github.com/user-attachments/assets/2ced559b-2418-4018-bd27-4401722ccf9f" width = "50%" height = "50%" >
#### 2.2. 사용 기술

- Virtual Box - 7.1.4
- Network Host/Ping Scan - Nmap
- Telnet, Pingflooding
- Python - v3.1
- QNX Software Center
- QNX Momentics IDE
- QNX Software Development Platform 8.0
- IAR Embedded Workbench for RISC-V - v3.1.4
- CLion - 2023.1.7

### 3. 설치 및 사용 방법
#### 3.1. IAR Embedded Workbench for RISC-V
- Server PC
  - 납품 이메일에서 서버툴 설치 링크를 클릭한다.
  - Installer를 클릭하여 설치를 시작한다.
  - 동글이 있는 경우, 동글을 PC에서 분리 후 OK버튼을 누른다.
  - 설치 경로, 폴더 이름을 지정하고 설치를 진행한다.
  - 라이센스 사용 계약을 확인하고 동의한다.
  - 사용자 정보 입력
  - 설치되는 경로를 지정하고 설치 타입은 기본 Complete로 설치한다.
  - 시스템 방화벽에 자동의 예외처리 추가를 허용하고 설치를 진행한다.
  - License Manager for Server를 실행한다.
  - License -> Activate License -> Online activation
  - 라이센스 번호를 '-'를 포함하여 입력한다.
  - 영구 라이센스 전환 메일을 수신하고 License -> Check for License Renewal를 클릭한다.
  - Server PC도 만약 IAR Embedded Workbench 가 필요하면 사용할 수 있다.
- Client PC
  - IAR Embedded Workbench를 설치한다.
  - 라이센스 사용 계약 확인 및 동의.
  - 설치되는 경로를 지정.
  - USB 드라이버의 설치를 설정.
  - 폴더 이름을 지정.
  - 설치가 완료되면 라이브러리 설치 안내에 YES를 클릭한다.
  - IAR Embedded Workbench를 실행하면 서버를 감지해서 연결창이 뜬다.
  - 안뜬다면 License -> Use Network License -> 연결할 서버를 선택해서 연결한다.
  - 연결할 서버가 없다면 Add a Server를 클릭해서 Server PC의 IP, 이름을 사용해서 서버를 등록하고 연결한다.
#### 3.2. QNX 
- Virtual Box 공식 홈페이지에서 Virtual Box를 설치한다.
- QNX 공식 홈페이지에서 QNX Software Center를 설치한다.
- QNX Software Center를 실행항 후 QNX Momentics IDE, QNX Software Development Platform 8.0을 설치한다.
- Virtual Box를 실행한 후 네트워킹 설정을 Virtual Box Host-Only로 설정한다.
- QNX Momentics IDE를 실행한 후 New Launch Target - QNX Virtual Machine Target - Target Name, VM Platform : vbox, CPU Architecture: x84_64로 설정한 후 Finish를 누르면 Virtual Box에서 QNX가 실행된다.
- 만약 실행이 되지 않는다면 컴퓨터의 BIOS 설정에 진입하여 CPU의 가상화 기술인 VT-x를 활성화한다.

### 4. 소개 및 시연 영상
[![졸업과제 48팀](http://img.youtube.com/vi/yt2jiEOSTUk&t=135s/0.jpeg)](https://www.youtube.com/watch?v=yt2jiEOSTUk&t=135s)

### 5. 팀 소개

<div align = "center">
  <table align = "center">
    <tr align = "center">
      <td>이름</td>
      <td>임준식</td>
      <td>정혁준</td>
      <td>김성문</td>
    </tr>
    <tr align = "center">
      <td>역할</td>
      <td>1. AES 암호 알고리즘 경량화 <br/>
          2. AES 암호 알고리즘 테스트벡터로 검증 <br/>
          3. 성능 측정<br/>
          4. 최종 보고서 작성
      </td>
      <td>
          1. QNX 취약점 분석<br/>
          2. ECC 암호 알고리즘 테스트벡터로 검증<br/>
          3. QNX 기존의 취약점 외에 다른 취약점 탐색
          3. 최종 보고서 작성<br/>
      </td>
      <td>
          1. ARIA 암호 알고리즘 테스트벡터로 검증<br/>
          2. 알고리즘 경량화 및 QNX 취약점 분석을 논문을 보고 방안 제시<br/>
          3. 최종 보고서 작성 <br/>
          4. 포스터 제작 <br/>
      </td>
    </tr>
   <tr align = "center"> 
     <td>연락처</td>
     <td>010-2559-9862</td>
     <td>010-5580-8464</td>
     <td>010-4691-2736</td>
   </tr>
   <tr align = "center">
     <td>깃허브</td>   
     <td> <a href = "https://github.com/rkrzy">임준식</a></td>
     <td> <a href = "https://github.com/juni0317">정혁준</a></td>
     <td> <a href = "https://github.com/doorcs">김성문</a></td>          
   </tr>
</table>
</div>

