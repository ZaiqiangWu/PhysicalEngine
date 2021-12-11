#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

#define GAMMA 5.8284271247
#define C_STAR 0.9238795325
#define S_STAR 0.3826834323
#define SVD_EPS 0.0000001

vec2 approx_givens_quat(float s_pp, float s_pq, float s_qq) {
    float c_h = 2 * (s_pp - s_qq);
    float s_h2 = s_pq * s_pq;
    float c_h2 = c_h * c_h;
    if (GAMMA * s_h2 < c_h2) {
        float omega = 1.0f / sqrt(s_h2 + c_h2);
        return vec2(omega * c_h, omega * s_pq);
    }
    return vec2(C_STAR, S_STAR);
}
// the quaternion is stored in vec4 like so:
// (c, s * vec3) meaning that .x = c
mat3 quat_to_mat3(vec4 quat) {
    float qx2 = quat.y * quat.y;
    float qy2 = quat.z * quat.z;
    float qz2 = quat.w * quat.w;
    float qwqx = quat.x * quat.y;
    float qwqy = quat.x * quat.z;
    float qwqz = quat.x * quat.w;
    float qxqy = quat.y * quat.z;
    float qxqz = quat.y * quat.w;
    float qyqz = quat.z * quat.w;

    return mat3(1.0f - 2.0f * (qy2 + qz2), 2.0f * (qxqy + qwqz), 2.0f * (qxqz - qwqy),
        2.0f * (qxqy - qwqz), 1.0f - 2.0f * (qx2 + qz2), 2.0f * (qyqz + qwqx),
        2.0f * (qxqz + qwqy), 2.0f * (qyqz - qwqx), 1.0f - 2.0f * (qx2 + qy2));
}

mat3 symmetric_eigenanalysis(mat3 A) {
    mat3 S = transpose(A) * A;
    // jacobi iteration
    mat3 q = mat3(1.0f);
    for (int i = 0; i < 5; i++) {
        vec2 ch_sh = approx_givens_quat(S[0].x, S[0].y, S[1].y);
        vec4 ch_sh_quat = vec4(ch_sh.x, 0, 0, ch_sh.y);
        mat3 q_mat = quat_to_mat3(ch_sh_quat);
        S = transpose(q_mat) * S * q_mat;
        q = q * q_mat;

        ch_sh = approx_givens_quat(S[0].x, S[0].z, S[2].z);
        ch_sh_quat = vec4(ch_sh.x, 0, -ch_sh.y, 0);
        q_mat = quat_to_mat3(ch_sh_quat);
        S = transpose(q_mat) * S * q_mat;
        q = q * q_mat;

        ch_sh = approx_givens_quat(S[1].y, S[1].z, S[2].z);
        ch_sh_quat = vec4(ch_sh.x, ch_sh.y, 0, 0);
        q_mat = quat_to_mat3(ch_sh_quat);
        S = transpose(q_mat) * S * q_mat;
        q = q * q_mat;

    }
    return q;
}

vec2 approx_qr_givens_quat(float a0, float a1) {
    float rho = sqrt(a0 * a0 + a1 * a1);
    float s_h = a1;
    float max_rho_eps = rho;
    if (rho <= SVD_EPS) {
        s_h = 0;
        max_rho_eps = SVD_EPS;
    }
    float c_h = max_rho_eps + a0;
    if (a0 < 0) {
        float temp = c_h - 2 * a0;
        c_h = s_h;
        s_h = temp;
    }
    float omega = 1.0f / sqrt(c_h * c_h + s_h * s_h);
    return vec2(omega * c_h, omega * s_h);
}

struct QR_mats {
    mat3 Q;
    mat3 R;
};

QR_mats qr_decomp(mat3 B) {
    QR_mats qr_decomp_result;
    mat3 R;
    // 1 0
    // (ch, 0, 0, sh)
    vec2 ch_sh10 = approx_qr_givens_quat(B[0].x, B[0].y);
    mat3 Q10 = quat_to_mat3(vec4(ch_sh10.x, 0, 0, ch_sh10.y));
    R = transpose(Q10) * B;

    // 2 0
    // (ch, 0, -sh, 0)
    vec2 ch_sh20 = approx_qr_givens_quat(R[0].x, R[0].z);
    mat3 Q20 = quat_to_mat3(vec4(ch_sh20.x, 0, -ch_sh20.y, 0));
    R = transpose(Q20) * R;

    // 2 1
    // (ch, sh, 0, 0)
    vec2 ch_sh21 = approx_qr_givens_quat(R[1].y, R[1].z);
    mat3 Q21 = quat_to_mat3(vec4(ch_sh21.x, ch_sh21.y, 0, 0));
    R = transpose(Q21) * R;

    qr_decomp_result.R = R;

    qr_decomp_result.Q = Q10 * Q20 * Q21;
    return qr_decomp_result;
}

struct SVD_mats {
    mat3 U;
    mat3 Sigma;
    mat3 V;
};

SVD_mats svd(mat3 A) {
    SVD_mats svd_result;
    svd_result.V = symmetric_eigenanalysis(A);

    mat3 B = A * svd_result.V;

    // sort singular values
    float rho0 = dot(B[0], B[0]);
    float rho1 = dot(B[1], B[1]);
    float rho2 = dot(B[2], B[2]);
    if (rho0 < rho1) {
        vec3 temp = B[1];
        B[1] = -B[0];
        B[0] = temp;
        temp = svd_result.V[1];
        svd_result.V[1] = -svd_result.V[0];
        svd_result.V[0] = temp;
        float temp_rho = rho0;
        rho0 = rho1;
        rho1 = temp_rho;
    }
    if (rho0 < rho2) {
        vec3 temp = B[2];
        B[2] = -B[0];
        B[0] = temp;
        temp = svd_result.V[2];
        svd_result.V[2] = -svd_result.V[0];
        svd_result.V[0] = temp;
        rho2 = rho0;
    }
    if (rho1 < rho2) {
        vec3 temp = B[2];
        B[2] = -B[1];
        B[1] = temp;
        temp = svd_result.V[2];
        svd_result.V[2] = -svd_result.V[1];
        svd_result.V[1] = temp;
    }

    QR_mats QR = qr_decomp(B);
    svd_result.U = QR.Q;
    svd_result.Sigma = QR.R;
    return svd_result;
}

struct UP_mats {
    mat3 U;
    mat3 P;
};

UP_mats SVD_to_polar(SVD_mats B) {
    UP_mats polar;
    polar.P = B.V * B.Sigma * transpose(B.V);
    polar.U = B.U * transpose(B.V);
    return polar;
}

UP_mats polar_decomp(mat3 A) {
    SVD_mats B = svd(A);
    UP_mats polar;
    polar.P = B.V * B.Sigma * transpose(B.V);
    polar.U = B.U * transpose(B.V);
    return polar;
}