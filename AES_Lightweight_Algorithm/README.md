# AES 경량화 알고리즘을 모아놓은 파일입니다.

1. AES128_CTR.h : AES128_CTR 모드의 암호 알고리즘
2. AES128_beforehand.h : AES 사전연산 기법
3. AES128_beforehand_light.h : byte수를 경량화한 사전연산 기법
4. AES128_RemoveShift.h : ShiftRows 제거 연산 기법
5. AES128_Mixcolumns_Light.h : MixColumns 사전 연산 기법
6. Common_Calculate.h : 공통된 연산을 모아놓은 파일
7. Conversion.h : SubByte를 수행할때 변환하는 값
8. main.c : 암호화 알고리즘 수행

# 경량화 성능 평가 (음수면 경량화가 아닌 증가한것임) (- = 시간 또는 Clock 수 증가)
1. 평문 : 6bc1bee22e409f96e93d7e117393172a (두글자 : 1바이트)
2. 키 : 2b7e151628aed2a6abf7158809cf4f3c (두글자 : 1바이트)
3. IV : f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff (두글자 : 1바이트)
<table border="1">
    <thead>
        <tr>
            <th>알고리즘</th>
            <th>AES_CTR_BE</th>
            <th>AES_CTR_BE_LIGHT</th>
            <th>AES_RE_SHIFTRW</th>
            <th>AES_MIX_LIGHT</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>CYC</td>
            <td>-0.7%</td>
            <td>1.3%</td>
            <td>-2.2%</td>
            <td>-0.2%</td>
        </tr>
        <tr>
            <td>TIME</td>
            <td>-0.4%</td>
            <td>1.4%</td>
            <td>-1.8%</td>
            <td>0.0%</td>
        </tr>
    </tbody>
</table>

---

1. 평문 : 6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52ef (두글자 : 1바이트)
2. 키 : 2b7e151628aed2a6abf7158809cf4f3c (두글자 : 1바이트)
3. IV : f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff (두글자 : 1바이트)

<table border="1">
    <thead>
        <tr>
            <th>알고리즘</th>
            <th>AES_CTR_BE</th>
            <th>AES_CTR_BE_LIGHT</th>
            <th>AES_RE_SHIFTRW</th>
            <th>AES_MIX_LIGHT</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>CYC</td>
            <td>-0.5%</td>
            <td>4.2%</td>
            <td>-2.3%</td>
            <td>-6.2%</td>
        </tr>
        <tr>
            <td>TIME</td>
            <td>-0.4%</td>
            <td>4.3%</td>
            <td>-2.4%</td>
            <td>6.3%</td>
        </tr>
    </tbody>
</table>

---

1. 평문 : 6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710f5d3d58503b9699de785895a96fdbaaf (두글자 : 1바이트)
2. 키 : 2b7e151628aed2a6abf7158809cf4f3c (두글자 : 1바이트)
3. IV : f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff (두글자 : 1바이트)

<table border="1">
    <thead>
        <tr>
            <th>알고리즘</th>
            <th>AES_CTR_BE</th>
            <th>AES_CTR_BE_LIGHT</th>
            <th>AES_RE_SHIFTRW</th>
            <th>AES_MIX_LIGHT</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>CYC</td>
            <td>-0.5%</td>
            <td>5.2%</td>
            <td>-2.4%</td>
            <td>-8.4%</td>
        </tr>
        <tr>
            <td>TIME</td>
            <td>-0.4%</td>
            <td>5.2%</td>
            <td>-2.4%</td>
            <td>8.3%</td>
        </tr>
    </tbody>
</table>

---

1. 평문 : 6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710f5d3d58503b9699de785895a96fdbaafe03a030d0f3fd6f3fe60734f6a03fdee0ba4a5f76d6cd0d5e64b225d26e119b77a85e5ed2073033d06cf6c6e4a59e5c1 (두글자 : 1바이트)
2. 키 : 2b7e151628aed2a6abf7158809cf4f3c (두글자 : 1바이트)
3. IV : f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff (두글자 : 1바이트)

<table border="1">
    <thead>
        <tr>
            <th>알고리즘</th>
            <th>AES_CTR_BE</th>
            <th>AES_CTR_BE_LIGHT</th>
            <th>AES_RE_SHIFTRW</th>
            <th>AES_MIX_LIGHT</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>CYC</td>
            <td>-0.4%</td>
            <td>5.9%</td>
            <td>-2.4%</td>
            <td>-9.9%</td>
        </tr>
        <tr>
            <td>TIME</td>
            <td>-0.4%</td>
            <td>5.9%</td>
            <td>-2.4%</td>
            <td>10.0%</td>
        </tr>
    </tbody>
</table>

---

1. 평문 :6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710f5d3d58503b9699de785895a96fdbaafe03a030d0f3fd6f3fe60734f6a03fdee0ba4a5f76d6cd0d5e64b225d26e119b77a85e5ed2073033d06cf6c6e4a59e5c1a5e46997257fe23365b94e40bdbdb14f4686cfbc7c7559d2f80d3a60c4c9d0ac (두글자 : 1바이트)
2. 키 : 2b7e151628aed2a6abf7158809cf4f3c (두글자 : 1바이트)
3. IV : f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff (두글자 : 1바이트)


<table border="1">
    <thead>
        <tr>
            <th>알고리즘</th>
            <th>AES_CTR_BE</th>
            <th>AES_CTR_BE_LIGHT</th>
            <th>AES_RE_SHIFTRW</th>
            <th>AES_MIX_LIGHT</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>CYC</td>
            <td>-0.4%</td>
            <td>6.2%</td>
            <td>-2.4%</td>
            <td>-10.5%</td>
        </tr>
        <tr>
            <td>TIME</td>
            <td>-0.4%</td>
            <td>6.1%</td>
            <td>-2.4%</td>
            <td>10.5%</td>
        </tr>
    </tbody>
</table>
