# gitman

Arduino IoT + Flask 서버 기반 자동화 도어/환경 제어 시스템.

## 구성

- **arduino/serverparser/**: 서버 명령을 파싱해 도어·장치를 제어하는 아두이노 펌웨어
- **arduino/serverrcv/**: 서버에서 데이터를 수신하는 아두이노 펌웨어
- **test01.py**: Flask 웹 서버 - 버튼 클릭 및 시간 예약으로 장치 제어, 상태 조회
- **templates/pr.html**: 제어 UI 웹 페이지

## 실행

```bash
pip install flask
python test01.py
```

서버 접속 후 버튼으로 도어 개폐 또는 시간 예약 설정.