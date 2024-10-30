#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

int modInverse(int a, int p);

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

// ECB 모드 암호화
void ecbEncrypt(const uint8_t* plaintext, uint8_t* ciphertext, const uint8_t* key, size_t length) {
    for (size_t i = 0; i < length; i += 16) {
        simpleAesEncrypt(plaintext + i, ciphertext + i, key); // 16바이트 블록 단위로 암호화
    }
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

    // ECB 암호화 예제
    uint8_t key[16] = "1234567890123456"; // 16바이트 키
    uint8_t plaintext[] = "Hello, World!!!"; // 평문
    size_t plaintextLength = sizeof(plaintext);
    size_t paddedLength = (plaintextLength + 15) / 16 * 16; // 16바이트 블록으로 패딩

    // 동적으로 메모리 할당
    uint8_t* paddedPlaintext = (uint8_t*)malloc(paddedLength * sizeof(uint8_t));
    if (paddedPlaintext == NULL) {
        perror("메모리 할당 실패");
        return EXIT_FAILURE;
    }

    // 평문 복사 및 패딩 추가
    memcpy(paddedPlaintext, plaintext, plaintextLength);
    memset(paddedPlaintext + plaintextLength, 0, paddedLength - plaintextLength); // 남은 부분 0으로 패딩

    // ciphertext도 동적으로 할당
    uint8_t* ciphertext = (uint8_t*)malloc(paddedLength * sizeof(uint8_t));
    if (ciphertext == NULL) {
        perror("메모리 할당 실패");
        free(paddedPlaintext); // 메모리 해제
        return EXIT_FAILURE;
    }

    ecbEncrypt(paddedPlaintext, ciphertext, key, paddedLength);

    printf("Ciphertext: ");
    for (size_t i = 0; i < paddedLength; i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");

    // 메모리 해제
    free(paddedPlaintext);
    free(ciphertext);

    clock_t end = clock();
    printf("소요 시간: %lf\n", (double)(end - start) / CLOCKS_PER_SEC);

    return 0;
}

