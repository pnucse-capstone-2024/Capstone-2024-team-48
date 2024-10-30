#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int a;
    int b;
    int p; // 소수
} EllipticCurve;

// 점 덧셈
Point add(Point P, Point Q, EllipticCurve curve) {
    if (P.x == 0 && P.y == 0) return Q;
    if (Q.x == 0 && Q.y == 0) return P;

    int lambda = (Q.y - P.y) * modInverse(Q.x - P.x, curve.p) % curve.p;
    int xR = (lambda * lambda - P.x - Q.x) % curve.p;
    int yR = (lambda * (P.x - xR) - P.y) % curve.p;

    return (Point) { (xR + curve.p) % curve.p, (yR + curve.p) % curve.p };
}

// 스칼라 곱셈
Point scalarMultiply(Point P, int k, EllipticCurve curve) {
    Point R = { 0, 0 }; // 무한대 점
    for (int i = 0; i < k; i++) {
        R = add(R, P, curve);
    }
    return R;
}

// 모듈로 역수 계산
int modInverse(int a, int p) {
    a = a % p;
    for (int x = 1; x < p; x++) {
        if ((a * x) % p == 1) {
            return x;
        }
    }
    return 1; // 역수가 없는 경우
}

// AES 암호화 (단순화된 형태)
void simpleAesEncrypt(const uint8_t* input, uint8_t* output, const uint8_t* key) {
    for (int i = 0; i < 16; i++) {
        output[i] = input[i] ^ key[i]; // XOR 연산으로 암호화
    }
}

// GCM 모드 암호화
void gcmEncrypt(const uint8_t* plaintext, uint8_t* ciphertext, const uint8_t* key, size_t length, uint8_t* iv, uint8_t* tag) {
    uint8_t outputBlock[16];
    uint8_t feedback[16];
    uint8_t authTag[16] = { 0 }; // 인증 태그 초기화
    size_t i;

    // 초기화 벡터를 피드백으로 사용
    memcpy(feedback, iv, 16);

    // GCM 암호화
    for (i = 0; i < length; i += 16) {
        simpleAesEncrypt(feedback, outputBlock, key);

        for (size_t j = 0; j < 16 && (i + j) < length; j++) {
            ciphertext[i + j] = plaintext[i + j] ^ outputBlock[j];
            authTag[j] ^= ciphertext[i + j]; // 인증 태그 업데이트
        }

        // 피드백을 다음 블록으로 업데이트
        for (int j = 15; j >= 0; j--) {
            if (++feedback[j] != 0) break;
        }
    }

    memcpy(tag, authTag, 16); // 최종 인증 태그 저장
}

int main() {
    clock_t start = clock();
    EllipticCurve curve = { 2, 3, 97 }; // y^2 = x^3 + 2x + 3 (mod 97)
    Point P = { 3, 6 };
    Point Q = { 10, 7 };

    Point R = add(P, Q, curve);
    printf("P + Q = (%d, %d)\n", R.x, R.y);

    int k = 2;
    Point S = scalarMultiply(P, k, curve);
    printf("2P = (%d, %d)\n", S.x, S.y);

    // GCM 암호화 예제
    uint8_t key[16] = "1234567890123456"; // 16바이트 키
    uint8_t plaintext[] = "Hello, World!!!"; // 평문
    size_t plaintextLength = sizeof(plaintext);
    uint8_t ciphertext[sizeof(plaintext)];
    uint8_t tag[16]; // 인증 태그

    // 초기화 벡터
    uint8_t iv[16] = "initialvector!!"; // 16바이트 IV

    // GCM 모드 사용하여 암호화
    gcmEncrypt(plaintext, ciphertext, key, plaintextLength, iv, tag);

    printf("Ciphertext: ");
    for (size_t i = 0; i < plaintextLength; i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");

    printf("Tag: ");
    for (size_t i = 0; i < 16; i++) {
        printf("%02x", tag[i]);
    }
    printf("\n");

    clock_t end = clock();
    printf("소요 시간: %lf\n", (double)(end - start) / CLOCKS_PER_SEC);

    return 0;
}
