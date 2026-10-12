# DreamofLions - 멀티플레이 숫자야구

UE 5.6 C++로 만든 2인 채팅 숫자야구입니다. (Listen Server)

## 구현 기능

- [x] 멀티플레이 채팅
- [x] 정답 생성 (1~9, 중복 없는 3자리, 서버에서 생성)
- [x] 입력 검사 (3자리 / 문자·0 / 중복 → 본인에게만 안내, 기회 유지)
- [x] 판정 (`1S1B`, `OUT`)
- [x] 시도 횟수 관리 (PlayerState, `[현재/최대]` Getter)
- [x] 승리·무승부 판정 + 공지 위젯 + 리셋
- [x] C++ 라이브러리 (`USLBaseballLibrary`)

## 흐름

```
[클라] 엔터 → PC::ServerSendInput (Server RPC)
[서버] GameMode::HandleChatInput → 검사 → 판정 → JudgeGame
[클라] PC::ClientReceiveChat / ClientShowNotice (Client RPC)
```

## 설계 결정

- **판정은 서버(GameMode)에서만**: GameMode는 클라에 없어서 정답을 조작할 수 없음
- **Multicast 대신 PC 순회 + Client RPC**: 입력 오류는 본인에게만 보내야 해서 Client RPC가 필요했고, 전체 방송도 같은 방식으로 통일
- **계산은 static 라이브러리로 분리**: 상태 없는 순수 계산이라 GameMode와 분리
- 강의 코드의 중복 검사 누락(`"112"` 통과)과 판정 중복 실행을 수정

## 실행

Number of Players 2, Net Mode: Play As Listen Server로 플레이.
정답은 Output Log의 `[Secret]`에서 확인할 수 있습니다.
