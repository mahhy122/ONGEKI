#include <Keyboard.h>

constexpr uint8_t NUM_KEYS = 6;
const uint8_t inputPins[NUM_KEYS] = {2, 3, 5, 19, 20, 21};
const uint8_t keyCodes[NUM_KEYS]  = {
  'k', 'j', 'h', 'g', 'f', 'd'  // 任意のキーバインド
};

constexpr unsigned long longPressThreshold = 500; // 長押しの判定閾値 (ms)

struct KeyState {
  bool current      = false;  // 現在の押下状態
  bool previous     = false;  // 前回の押下状態
  unsigned long pressedAt = 0;    // 押下時刻
  bool longActive  = false;  // 長押し判定済か
} ks[NUM_KEYS];

void setup() {
  for (uint8_t i = 0; i < NUM_KEYS; ++i) {
    pinMode(inputPins[i], INPUT_PULLUP); // GNDに落とすスイッチを想定
  }
  Keyboard.begin();
}

void loop() {
  const unsigned long now = millis();

  for (uint8_t i = 0; i < NUM_KEYS; ++i) {
    ks[i].current = !digitalRead(inputPins[i]); // LOW = 押下

    // 押された瞬間
    if (ks[i].current && !ks[i].previous) {
      ks[i].pressedAt  = now;
      ks[i].longActive = false;
      Keyboard.press(keyCodes[i]);
    }

    // 長押ししきい値に達したら（必要であればここで処理可能）
    if (ks[i].current && !ks[i].longActive &&
        (now - ks[i].pressedAt >= longPressThreshold)) {
      ks[i].longActive = true;
    }

    // 離された瞬間
    if (!ks[i].current && ks[i].previous) {
      Keyboard.release(keyCodes[i]);
    }

    ks[i].previous = ks[i].current; // 状態更新
  }

  delayMicroseconds(500); // USBポーリング
}
