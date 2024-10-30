#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int a;
    int b;
    int p; // 소수
} EllipticCurve;

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

// 헥사 문자열을 정수로 변환
int hexToInt(const char* hex) {
    int value;
    sscanf_s(hex, "%x", &value); // 안전한 함수 사용
    return value;
}

int main() {
    clock_t start = clock();
    EllipticCurve curve = { 2, 3, 97 }; // y^2 = x^3 + 2x + 3 (mod 97)

    // 기본 점 G 설정 (예: G = (3, 6))
    Point G = { 3, 6 };

    // 개인키 입력
    int privateKey;
    printf("개인키를 입력하세요: ");
    scanf_s("%d", &privateKey);

    // 공개키 계산
    Point publicKey = scalarMultiply(G, privateKey, curve);
    printf("공개키 P = (%d, %d)\n", publicKey.x, publicKey.y);

    // 파일에서 테스트 벡터 읽기
    FILE* file = fopen("C:\\Users\\A\\Desktop\\Projects\\ecc_test.txt", "r");
    if (!file) {
        perror("파일 열기 오류");
        return EXIT_FAILURE;
    }

    char line[256];
    Point QCA, QUT;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "QCAVSx")) {
            char* value = strchr(line, '=') + 1;
            QCA.x = hexToInt(value);
        }
        else if (strstr(line, "QCAVSy")) {
            char* value = strchr(line, '=') + 1;
            QCA.y = hexToInt(value);
        }
        else if (strstr(line, "QIUTx")) {
            char* value = strchr(line, '=') + 1;
            QUT.x = hexToInt(value);
        }
        else if (strstr(line, "QIUTy")) {
            char* value = strchr(line, '=') + 1;
            QUT.y = hexToInt(value);
        }
    }

    fclose(file);

    // 점 덧셈 수행
    Point R = add(QCA, QUT, curve);
    printf("P + Q = (%d, %d)\n", R.x, R.y);

    clock_t end = clock();
    printf("소요 시간: %lf\n", (double)(end - start) / CLOCKS_PER_SEC);

    return 0;
}
