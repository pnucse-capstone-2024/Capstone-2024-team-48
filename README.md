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
#### 2.1. 시스템 구성도
> 시스템 구성도(infra, front, back등의 node 간의 관계)의 사진을 삽입하세요.

#### 2.1. 사용 기술

- Virtual Box - 7.14
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
> 프로젝트에 대한 소개와 시연 영상을 넣으세요.

### 5. 팀 소개
> 팀원 소개 & 구성원 별 역할 분담 & 간단한 연락처를 작성하세요.
