#include"MathUtilty.h"


// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;

	return result;
}

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v) {
	Vector3 result;
	result.x = scalar * v.x;
	result.y = scalar * v.y;
	result.z = scalar * v.z;
	return result;
}

// 内積
float Dot(const Vector3& v1, const Vector3& v2) {
	float result;
	result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	return result;
}

// 長さ(ノルム)
float Length(const Vector3& v) {
	float result;
	result = powf(v.x, 2.0) + powf(v.y, 2.0) + powf(v.z, 2.0);

	return sqrtf(result);
};

// 正規化
Vector3 Normalize(const Vector3& v) {
	Vector3 result;
	float x;
	x = Length(v);
	result.x = v.x / x;
	result.y = v.y / x;
	result.z = v.z / x;
	return result;
}

// 1.行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 resultAdd;
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			resultAdd.m[row][column] = m1.m[row][column] + m2.m[row][column];
		}
	}
	return resultAdd;
}

// 2.行列の減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 resultSubtract;
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			resultSubtract.m[row][column] = m1.m[row][column] - m2.m[row][column];
		}
	}
	return resultSubtract;
}

// 行列の掛け算の関数
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 resultMultiply;
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			resultMultiply.m[row][column] =
				m1.m[row][0] * m2.m[0][column] + m1.m[row][1] * m2.m[1][column] +
				m1.m[row][2] * m2.m[2][column] + m1.m[row][3] * m2.m[3][column];
		}
	}
	return resultMultiply;
}

// 4.逆行列
Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 result;
	float formula = m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] +
		m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] +
		m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] -

		m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
		m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] -
		m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] -

		m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] -
		m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] -
		m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] +

		m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] +
		m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] +
		m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +

		m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] +
		m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] +
		m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] -

		m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] -
		m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] -
		m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] -

		m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] -
		m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
		m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] +

		m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] +
		m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] +
		m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];
	assert(formula != 0.0f);
	float formulaRec = 1.0f / formula;

	result.m[0][0] = (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] +
		m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] -
		m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]) *
		formulaRec;
	result.m[0][1] = (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] -
		m.m[0][3] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[2][2] * m.m[3][1] +
		m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]) *
		formulaRec;
	result.m[0][2] = (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] +
		m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] -
		m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]) *
		formulaRec;
	result.m[0][3] = (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] -
		m.m[0][3] * m.m[1][1] * m.m[2][2] + m.m[0][3] * m.m[1][2] * m.m[2][1] +
		m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]) *
		formulaRec;

	result.m[1][0] = (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * /**/ m.m[3][0] -
		m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[1][3] * m.m[2][2] * m.m[3][0] +
		m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]) *
		formulaRec;
	result.m[1][1] = (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] +
		m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] -
		m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]) *
		formulaRec;
	result.m[1][2] = (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] -
		m.m[0][3] * m.m[1][0] * m.m[3][2] + m.m[0][3] * m.m[1][2] * m.m[3][0] +
		m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]) *
		formulaRec;
	result.m[1][3] = (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] +
		m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] -
		m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]) *
		formulaRec;

	result.m[2][0] = (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] +
		m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] -
		m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]) *
		formulaRec;
	result.m[2][1] = (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] -
		m.m[0][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[2][1] * m.m[3][0] +
		m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]) *
		formulaRec;
	result.m[2][2] = (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] +
		m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] -
		m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]) *
		formulaRec;
	result.m[2][3] = (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] -
		m.m[0][3] * m.m[1][0] * m.m[2][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] +
		m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]) *
		formulaRec;

	result.m[3][0] = (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] -
		m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[1][2] * m.m[3][2] * m.m[3][0] +
		m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]) *
		formulaRec;
	result.m[3][1] = (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] +
		m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] -
		m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]) *
		formulaRec;
	result.m[3][2] = (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] -
		m.m[0][2] * m.m[1][0] * m.m[3][1] + m.m[0][2] * m.m[1][1] * m.m[3][0] +
		m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]) *
		formulaRec;
	result.m[3][3] = (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] +
		m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] -
		m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]) *
		formulaRec;

	return result;
}

////5.転置行列
Matrix4x4 Transpose(const Matrix4x4& m) {
	Matrix4x4 result;
	result.m[0][0] = m.m[0][0];
	result.m[0][1] = m.m[1][0];
	result.m[0][2] = m.m[2][0];
	result.m[0][3] = m.m[3][0];

	result.m[1][0] = m.m[0][1];
	result.m[1][1] = m.m[1][1];
	result.m[1][2] = m.m[2][1];
	result.m[1][3] = m.m[3][1];

	result.m[2][0] = m.m[0][2];
	result.m[2][1] = m.m[1][2];
	result.m[2][2] = m.m[2][2];
	result.m[2][3] = m.m[3][2];

	result.m[3][0] = m.m[0][3];
	result.m[3][1] = m.m[1][3];
	result.m[3][2] = m.m[2][3];
	result.m[3][3] = m.m[3][3];

	return result;
}
////6.単位行列の作成
Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 result;
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

// 平行移動(translate)
// 平行移動行列の関数
Matrix4x4 MakeTranselateMatrix(const Vector3& transelate) {
	Matrix4x4 result;
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;

	result.m[3][0] = transelate.x;
	result.m[3][1] = transelate.y;
	result.m[3][2] = transelate.z;
	result.m[3][3] = 1.0f;

	return result;
}

// 拡大縮小(scale)
// 拡大縮小行列の関数
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result;
	result.m[0][0] = scale.x;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = scale.y;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = scale.z;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

// 回転(rotate)

// x軸回転行列の関数
Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 resultMakeRotatedMatrix;
	resultMakeRotatedMatrix.m[0][0] = 1;
	resultMakeRotatedMatrix.m[0][1] = 0;
	resultMakeRotatedMatrix.m[0][2] = 0;
	resultMakeRotatedMatrix.m[0][3] = 0;

	resultMakeRotatedMatrix.m[1][0] = 0;
	resultMakeRotatedMatrix.m[1][1] = std::cos(radian);
	resultMakeRotatedMatrix.m[1][2] = std::sin(radian);
	resultMakeRotatedMatrix.m[1][3] = 0;

	resultMakeRotatedMatrix.m[2][0] = 0;
	resultMakeRotatedMatrix.m[2][1] = -std::sin(radian);
	resultMakeRotatedMatrix.m[2][2] = std::cos(radian);
	resultMakeRotatedMatrix.m[2][3] = 0;

	resultMakeRotatedMatrix.m[3][0] = 0;
	resultMakeRotatedMatrix.m[3][1] = 0;
	resultMakeRotatedMatrix.m[3][2] = 0;
	resultMakeRotatedMatrix.m[3][3] = 1;

	return resultMakeRotatedMatrix;
}

// y軸回転行列の関数
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 resultMakeRotatedMatrix;
	resultMakeRotatedMatrix.m[0][0] = std::cos(radian);
	resultMakeRotatedMatrix.m[0][1] = 0;
	resultMakeRotatedMatrix.m[0][2] = -std::sin(radian);
	resultMakeRotatedMatrix.m[0][3] = 0;

	resultMakeRotatedMatrix.m[1][0] = 0;
	resultMakeRotatedMatrix.m[1][1] = 1;
	resultMakeRotatedMatrix.m[1][2] = 0;
	resultMakeRotatedMatrix.m[1][3] = 0;

	resultMakeRotatedMatrix.m[2][0] = std::sin(radian);
	resultMakeRotatedMatrix.m[2][1] = 0;
	resultMakeRotatedMatrix.m[2][2] = std::cos(radian);
	resultMakeRotatedMatrix.m[2][3] = 0;

	resultMakeRotatedMatrix.m[3][0] = 0;
	resultMakeRotatedMatrix.m[3][1] = 0;
	resultMakeRotatedMatrix.m[3][2] = 0;
	resultMakeRotatedMatrix.m[3][3] = 1;

	return resultMakeRotatedMatrix;
}

// z軸回転行列の関数
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 resultMakeRotatedMatrix;
	resultMakeRotatedMatrix.m[0][0] = std::cos(radian);
	resultMakeRotatedMatrix.m[0][1] = std::sin(radian);
	resultMakeRotatedMatrix.m[0][2] = 0;
	resultMakeRotatedMatrix.m[0][3] = 0;

	resultMakeRotatedMatrix.m[1][0] = -std::sin(radian);
	resultMakeRotatedMatrix.m[1][1] = std::cos(radian);
	resultMakeRotatedMatrix.m[1][2] = 0;
	resultMakeRotatedMatrix.m[1][3] = 0;

	resultMakeRotatedMatrix.m[2][0] = 0;
	resultMakeRotatedMatrix.m[2][1] = 0;
	resultMakeRotatedMatrix.m[2][2] = 1;
	resultMakeRotatedMatrix.m[2][3] = 0;

	resultMakeRotatedMatrix.m[3][0] = 0;
	resultMakeRotatedMatrix.m[3][1] = 0;
	resultMakeRotatedMatrix.m[3][2] = 0;
	resultMakeRotatedMatrix.m[3][3] = 1;

	return resultMakeRotatedMatrix;
}

// 3次元アフィン変換行列の関数
Matrix4x4 MakeAffinMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 resultMakeAffinMatrix;
	Matrix4x4 resultMakeScaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 resultMakeTranselateMatrix = MakeTranselateMatrix(translate);
	Matrix4x4 resultMakeRotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 resultMakeRotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 resultMakeRotateZMatrix = MakeRotateZMatrix(rotate.z);

	Matrix4x4 rotateXYZMatrix = Multiply(
		resultMakeRotateXMatrix, Multiply(resultMakeRotateYMatrix, resultMakeRotateZMatrix));

	resultMakeAffinMatrix =
		Multiply(resultMakeScaleMatrix, Multiply(rotateXYZMatrix, resultMakeTranselateMatrix));

	return resultMakeAffinMatrix;
}

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result = {
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
	};

	return result;
}

Vector3 SumVector3(Vector3& num1, Vector3& num2) {
	Vector3 result{};
	result.x = num1.x += num2.x;
	result.y = num1.y += num2.y;
	result.z = num1.z += num2.z;
	return result;
}

//1.透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float forY, float aspectRatio, float nearClip, float farClip)
{
	Matrix4x4 result;

	result.m[0][0] = 1 / aspectRatio * (1 / std::tan(forY / 2));
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = 1 / std::tan(forY / 2);
	result.m[1][2] = 0;
	result.m[1][3] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = farClip / (farClip - nearClip);
	result.m[2][3] = 1;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);
	result.m[3][3] = 0;

	return result;

}

//2.正射影行列
Matrix4x4 MakeOrthographicmatrix(float left, float top, float right, float bottom, float nearClip, float farClip)
{
	Matrix4x4 result;

	result.m[0][0] = 2 / (right - left);
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = 2 / (top - bottom);
	result.m[1][2] = 0;
	result.m[1][3] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = 1 / (farClip - nearClip);
	result.m[2][3] = 0;

	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = nearClip / (nearClip - farClip);
	result.m[3][3] = 1;

	return result;

}

//3.ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
	Matrix4x4 result;

	result.m[0][0] = width / 2;
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = -height / 2;
	result.m[1][2] = 0;
	result.m[1][3] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = maxDepth - minDepth;
	result.m[2][3] = 0;

	result.m[3][0] = left + (width / 2);
	result.m[3][1] = top + (height / 2);
	result.m[3][2] = minDepth;
	result.m[3][3] = 1;

	return result;

}


// 線形補間
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 P;
	P.x = v1.x + t * (v2.x - v1.x);
	P.y = v1.y + t * (v2.y - v1.y);
	P.z = v1.z + t * (v2.z - v1.z);
	return P;
}

// 球面線形補間
Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t) {

	float angle = std::acos(Dot(v1, v2));

	float sinTheta = std::sin(angle);

	float PositonStart = std::sin(angle * (1 - t));
	float PositonEnd = std::sin(angle * t);

	Vector3 result;
	result.x = (PositonStart * v1.x + PositonEnd * v2.x) / sinTheta;
	result.y = (PositonStart * v1.y + PositonEnd * v2.y) / sinTheta;
	result.z = (PositonStart * v1.z + PositonEnd * v2.z) / sinTheta;

	return result;

}

// 3.座標変換
Vector3 CoorTransform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
		1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
		1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
		1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
		1.0f * matrix.m[3][3];

	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

Vector3 CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t)
{
	const float s = 0.5f;

	float t2 = t * t;	//tの2乗
	float t3 = t2 * t;	//tの3乗

	Vector3 e3;
	e3.x = -p0.x + 3 * p1.x - 3 * p2.x + p3.x;
	e3.y = -p0.y + 3 * p1.y - 3 * p2.y + p3.y;
	e3.z = -p0.z + 3 * p1.z - 3 * p2.z + p3.z;

	Vector3 e2;
	e2.x = 2 * p0.x - 5 * p1.x + 4 * p2.x - p3.x;
	e2.y = 2 * p0.y - 5 * p1.y + 4 * p2.y - p3.y;
	e2.z = 2 * p0.z - 5 * p1.z + 4 * p2.z - p3.z;

	Vector3 e1;
	e1.x = -p0.x + p2.x;
	e1.y = -p0.y + p2.y;
	e1.z = -p0.z + p2.z;

	Vector3 e0;
	e0.x = 2 * p1.x;
	e0.y = 2 * p1.y;
	e0.z = 2 * p1.z;

	Vector3 result;

	result.x = s * (e3.x * t3 + e2.x * t2 + e1.x * t + e0.x);
	result.y = s * (e3.y * t3 + e2.y * t2 + e1.y * t + e0.y);
	result.z = s * (e3.z * t3 + e2.z * t2 + e1.z * t + e0.z);

	return result;
}

Vector3 CatmullRomPosition(const std::vector<Vector3>& points, float t)
{
	assert(points.size() >= 4 && "制御点は4点以上必要です");
	
	size_t division = points.size() - 1;
	float areaWidth = 1.0f / division;
	float t_2 = std::fmod(t, areaWidth) * division;
	t_2 = std::clamp(t_2, 0.0f, 1.0f);

	//区間番号
	size_t index = static_cast<size_t>(t / areaWidth);
	//区間番号が上限を超えないように収める
	index = std::min(index, division - 1);

	size_t index0 = index - 1;
	size_t index1 = index;
	size_t index2 = index + 1;
	size_t index3 = index + 2;

	if (index == 0)
	{
		index0 == index1;
	}

	if (index3 >= points.size())
	{
		index3 = index2;
	}

	const Vector3& p0 = points[index0];
	const Vector3& p1 = points[index1];
	const Vector3& p2 = points[index2];
	const Vector3& p3 = points[index3];

	return CatmullRomInterpolation(p0, p1, p2, p3, t_2);
	
}


