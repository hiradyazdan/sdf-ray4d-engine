#pragma once

#include "nodeEditor_old/NodeData.hpp"
#include "SDFGraph/ExpressionEvaluator.hpp"

/// \file DistanceFieldData.hpp
/// \brief All the data type specific classes and structs.
///        Some are built around the NodeData class by Dimitry Pinaev [https://github.com/paceholder/nodeeditor]
/// \authors Teemu Lindborg & Phil Gifford
/// \version 1.0
/// \date 22/01/17 Updated to NCCA Coding standard

struct Vec4f
{
  Vec4f() {}
  Vec4f(const std::string &_x, const std::string &_y, const std::string &_z, const std::string &_w) :
    m_x(_x),
    m_y(_y),
    m_z(_z),
    m_w(_w)
  {}


  Vec4f operator +(const Vec4f &_rhs) {
    m_x = _rhs.m_x;
    m_y = _rhs.m_y;
    m_z = _rhs.m_z;
    m_w = _rhs.m_w;

    return *this;
  }

  void operator =(const Vec4f &_rhs) {
    m_x = _rhs.m_x;
    m_y = _rhs.m_y;
    m_z = _rhs.m_z;
    m_w = _rhs.m_w;
  }

	std::string m_x = "0.0000";
	std::string m_y = "0.0000";
	std::string m_z = "0.0000";
	std::string m_w = "1.0000";
};

enum DFNodeType
{
  PRIMITIVE,
  TRANSFORM,
  MIX,
  VECTOR,
  SCALAR,
	COLOR,
	IO,
	COLLAPSED,
	COPY
};

class Mat4f
{
public:
	Mat4f() : m_cpn(-1) {}
  Mat4f(const std::string &_m00, const std::string &_m10, const std::string &_m20, const std::string &_m30,
        const std::string &_m01, const std::string &_m11, const std::string &_m21, const std::string &_m31,
        const std::string &_m02, const std::string &_m12, const std::string &_m22, const std::string &_m32,
        const std::string &_m03, const std::string &_m13, const std::string &_m23, const std::string &_m33)
	 : m_cpn(-1)
  {
    m_m4f[0][0] = _m00;
    m_m4f[0][1] = _m01;
    m_m4f[0][2] = _m02;
    m_m4f[0][3] = _m03;

    m_m4f[1][0] = _m10;
    m_m4f[1][1] = _m11;
    m_m4f[1][2] = _m12;
    m_m4f[1][3] = _m13;

    m_m4f[2][0] = _m20;
    m_m4f[2][1] = _m21;
    m_m4f[2][2] = _m22;
    m_m4f[2][3] = _m23;

    m_m4f[3][0] = _m30;
    m_m4f[3][1] = _m31;
    m_m4f[3][2] = _m32;
    m_m4f[3][3] = _m33;
	}
	~Mat4f() {}

	Mat4f& operator=(const Mat4f& _m) noexcept
	{
		m_m4f[0][0] = _m.m_m4f[0][0];
		m_m4f[0][1] = _m.m_m4f[0][1];
		m_m4f[0][2] = _m.m_m4f[0][2];
		m_m4f[0][3] = _m.m_m4f[0][3];

		m_m4f[1][0] = _m.m_m4f[1][0];
		m_m4f[1][1] = _m.m_m4f[1][1];
		m_m4f[1][2] = _m.m_m4f[1][2];
		m_m4f[1][3] = _m.m_m4f[1][3];

		m_m4f[2][0] = _m.m_m4f[2][0];
		m_m4f[2][1] = _m.m_m4f[2][1];
		m_m4f[2][2] = _m.m_m4f[2][2];
		m_m4f[2][3] = _m.m_m4f[2][3];

		m_m4f[3][0] = _m.m_m4f[3][0];
		m_m4f[3][1] = _m.m_m4f[3][1];
		m_m4f[3][2] = _m.m_m4f[3][2];
		m_m4f[3][3] = _m.m_m4f[3][3];

		return *this;
	}

	Mat4f operator*(const Mat4f& _m) const noexcept
	{
		Mat4f temp;
		std::ostringstream row;

		row << "( " << m_m4f[0][0] << " ) * ( " << _m.m_m4f[0][0] << " ) + ( " << m_m4f[1][0] << " ) * ( " << _m.m_m4f[0][1] << " ) + ( " << m_m4f[2][0] << " ) * ( " << _m.m_m4f[0][2] << " ) + ( " << m_m4f[3][0] << " ) * ( " << _m.m_m4f[0][3] << " )";
		temp.m_m4f[0][0] = hsitho::Expressions::evaluate(row.str(), "", m_cpn);
		row.clear();
		row.str("");
		row << "( " << m_m4f[0][1] << " ) * ( " << _m.m_m4f[0][0] << " ) + ( " << m_m4f[1][1] << " ) * ( " << _m.m_m4f[0][1] << " ) + ( " << m_m4f[2][1] << " ) * ( " << _m.m_m4f[0][2] << " ) + ( " << m_m4f[3][1] << " ) * ( " << _m.m_m4f[0][3] << " )";
		temp.m_m4f[0][1] = hsitho::Expressions::evaluate(row.str(), "", m_cpn);
		row.clear();
		row.str("");
		row << "( " << m_m4f[0][2] << " ) * ( " << _m.m_m4f[0][0] << " ) + ( " << m_m4f[1][2] << " ) * ( " << _m.m_m4f[0][1] << " ) + ( " << m_m4f[2][2] << " ) * ( " << _m.m_m4f[0][2] << " ) + ( " << m_m4f[3][2] << " ) * ( " << _m.m_m4f[0][3] << " )";
		temp.m_m4f[0][2] = hsitho::Expressions::evaluate(row.str(), "", m_cpn);
		row.clear();
		row.str("");
		row << "( " << m_m4f[0][3] << " ) * ( " << _m.m_m4f[0][0] << " ) + ( " << m_m4f[1][3] << " ) * ( " << _m.m_m4f[0][1] << " ) + ( " << m_m4f[2][3] << " ) * ( " << _m.m_m4f[0][2] << " ) + ( " << m_m4f[3][3] << " ) * ( " << _m.m_m4f[0][3] << " )";
		temp.m_m4f[0][3] = hsitho::Expressions::evaluate(row.str(), "", m_cpn);
		row.clear();
		row.str("");
		row << "( " << m_m4f[0][0] << " ) * ( " << _m.m_m4f[1][0] << " ) + ( " << m_m4f[1][0] << " ) * ( " << _m.m_m4f[1][1] << " ) + ( " << m_m4f[2][0] << " ) * ( " << _m.m_m4f[1][2] << " ) + ( " << m_m4f[3][0] << " ) * ( " << _m.m_m4f[1][3] << " )";
		temp.m_m4f[1][0] = hsitho::Expressions::evaluate(row.str(), "", m_cpn);
		row.clear();
		row.str("");
		row << "( " << m_m4f[0][1] << " ) * ( " << _m.m_m4f[1][0] << " ) + ( " << m_m4f[1][1] << " ) * ( " << _m.m_m4f[1][1] << " ) + ( " << m_m4f[2][1] << " ) * ( " << _m.m_m4f[1][2] << " ) + ( " << m_m4f[3][1] << " ) * ( " << _m.m_m4f[1][3] << " )";
		temp.m_m4f[1][1] = hsitho::Expressions::evaluate(row.str(), "", m_cpn);
		row.clear();
		row.str("");
		row << "( " << m_m4f[0][2] << " ) * ( " << _m.m_m4f[1][0] << " ) + ( " << m_m4f[1][2] << " ) * ( " << _m.m_m4f[1][1] << " ) + ( " << m_m4f[2][2] << " ) * ( " << _m.m_m4f[1][2] << " ) + ( " << m_m4f[3][2] << " ) * ( " << _m.m_m4f[1][3] << " )";
		temp.m_m4f[1][2] = hsitho::Expressions::evaluate(row.str(), "", m_cpn);
		row.clear();
		row.str("");
		row << "( " << m_m4f[0][3] << " ) * ( " << _m.m_m4f[1][0] << " ) + ( " << m_m4f[1][3] << " ) * ( " << _m.m_m4f[1][1] << " ) + ( " << m_m4f[2][3] << " ) * ( " << _m.m_m4f[1][2] << " ) + ( " << m_m4f[3][3] << " ) * ( " << _m.m_m4f[1][3] << " )";
		temp.m_m4f[1][3] = hsitho::Expressions::evaluate(row.str(), "", m_cpn);
		row.clear();
		row.str("");
		row << "( " << m_m4f[0][0] << " ) * ( " << _m.m_m4f[2][0] << " ) + ( " << m_m4f[1][0] << " ) * ( " << _m.m_m4f[2][1] << " ) + ( " << m_m4f[2][0] << " ) * ( " << _m.m_m4f[2][2] << " ) + ( " << m_m4f[3][0] << " ) * ( " << _m.m_m4f[2][3] << " )";
		temp.m_m4f[2][0] = hsitho::Expressions::evaluate(row.str(), "", m_cpn);
		row.clear();
		row.str("");
		row << "( " << m_m4f[0][1] << " ) * ( " << _m.m_m4f[2][0] << " ) + ( " << m_m4f[1][1] << " ) * ( " << _m.m_m4f[2][1] << " ) + ( " << m_m4f[2][1] << " ) * ( " << _m.m_m4f[2][2] << " ) + ( " << m_m4f[3][1] << " ) * ( " << _m.m_m4f[2][3] << " )";
		temp.m_m4f[2][1] = hsitho::Expressions::evaluate(row.str(), "", m_cpn);
		row.clear();
		row.str("");
		row << "( " << m_m4f[0][2] << " ) * ( " << _m.m_m4f[2][0] << " ) + ( " << m_m4f[1][2] << " ) * ( " << _m.m_m4f[2][1] << " ) + ( " << m_m4f[2][2] << " ) * ( " << _m.m_m4f[2][2] << " ) + ( " << m_m4f[3][2] << " ) * ( " << _m.m_m4f[2][3] << " )";
		temp.m_m4f[2][2] = hsitho::Expressions::evaluate(row.str(), "", m_cpn);
		row.clear();
		row.str("");
		row << "( " << m_m4f[0][3] << " ) * ( " << _m.m_m4f[2][0] << " ) + ( " << m_m4f[1][3] << " ) * ( " << _m.m_m4f[2][1] << " ) + ( " << m_m4f[2][3] << " ) * ( " << _m.m_m4f[2][2] << " ) + ( " << m_m4f[3][3] << " ) * ( " << _m.m_m4f[2][3] << " )";
		temp.m_m4f[2][3] = hsitho::Expressions::evaluate(row.str(), "", m_cpn);
		row.clear();
		row.str("");
		row << "( " << m_m4f[0][0] << " ) * ( " << _m.m_m4f[3][0] << " ) + ( " << m_m4f[1][0] << " ) * ( " << _m.m_m4f[3][1] << " ) + ( " << m_m4f[2][0] << " ) * ( " << _m.m_m4f[3][2] << " ) + ( " << m_m4f[3][0] << " ) * ( " << _m.m_m4f[3][3] << " )";
		temp.m_m4f[3][0] = hsitho::Expressions::evaluate(row.str(), "", m_cpn);
		row.clear();
		row.str("");
		row << "( " << m_m4f[0][1] << " ) * ( " << _m.m_m4f[3][0] << " ) + ( " << m_m4f[1][1] << " ) * ( " << _m.m_m4f[3][1] << " ) + ( " << m_m4f[2][1] << " ) * ( " << _m.m_m4f[3][2] << " ) + ( " << m_m4f[3][1] << " ) * ( " << _m.m_m4f[3][3] << " )";
		temp.m_m4f[3][1] = hsitho::Expressions::evaluate(row.str(), "", m_cpn);
		row.clear();
		row.str("");
		row << "( " << m_m4f[0][2] << " ) * ( " << _m.m_m4f[3][0] << " ) + ( " << m_m4f[1][2] << " ) * ( " << _m.m_m4f[3][1] << " ) + ( " << m_m4f[2][2] << " ) * ( " << _m.m_m4f[3][2] << " ) + ( " << m_m4f[3][2] << " ) * ( " << _m.m_m4f[3][3] << " )";
		temp.m_m4f[3][2] = hsitho::Expressions::evaluate(row.str(), "", m_cpn);
		row.clear();
		row.str("");
		row << "( " << m_m4f[0][3] << " ) * ( " << _m.m_m4f[3][0] << " ) + ( " << m_m4f[1][3] << " ) * ( " << _m.m_m4f[3][1] << " ) + ( " << m_m4f[2][3] << " ) * ( " << _m.m_m4f[3][2] << " ) + ( " << m_m4f[3][3] << " ) * ( " << _m.m_m4f[3][3] << " )";
		temp.m_m4f[3][3] = hsitho::Expressions::evaluate(row.str(), "", m_cpn);

		return temp;
	}

	bool operator==(const Mat4f& _m) noexcept
	{
		for(unsigned int x = 0; x < 4; ++x)
		{
			for(unsigned int y = 0; y < 4; ++y)
			{
				if(m_m4f[x][y] != _m.m_m4f[x][y])
					return false;
			}
		}
		return true;
	}

	std::string matrix(int _x, int _y) const { return m_m4f[_x][_y]; }

	void print() const {
		for(int y = 0; y < 4; ++y)
		{
			for(int x = 0; x < 4; ++x)
			{
				std::cout << m_m4f[x][y] << ", ";
			}
			std::cout << "\n";
		}
	}

	void print(const Mat4f &_m) const {
		for(int y = 0; y < 4; ++y)
		{
			for(int x = 0; x < 4; ++x)
			{
				std::cout << _m.m_m4f[x][y] << " ";
			}
			std::cout << "\n";
		}
	}

	void setCpn(const int &_cpn) { m_cpn = _cpn; }

private:
	int m_cpn;
	std::string m_m4f[4][4] = {
		{"1.0", "0.0", "0.0", "0.0"},
		{"0.0", "1.0", "0.0", "0.0"},
		{"0.0", "0.0", "1.0", "0.0"},
		{"0.0", "0.0", "0.0", "1.0"}
	};
};


class DistanceFieldInput : public NodeData
{
public:
  NodeDataType type() const override
  {
    return NodeDataType {"DistanceFieldData", "     ", Qt::cyan};
  }
};

class DistanceFieldOutput : public NodeData
{
public:
  NodeDataType type() const override
  {
    return NodeDataType {"DistanceFieldData", "Result", Qt::cyan};
  }
};

class ScalarData : public NodeData
{
public:
	ScalarData() : m_id(" "), m_value("0.0") {}
	ScalarData(const std::string &_v) : m_id(" "), m_value(_v) {}
	ScalarData(const std::string &_id, const std::string &_v) : m_id(_id), m_value(_v) {}
	NodeDataType type() const override
	{
		return NodeDataType {"Scalar", m_id.c_str(), Qt::red};
	}
	std::string value() const { return m_value; }
private:
	std::string m_id;
	std::string m_value;
};

class VectorData : public NodeData
{
public:
	VectorData(const std::string &_id = "Vec") : m_v(Vec4f()), m_id(_id) {}
	VectorData(const std::string &_x, const std::string &_y, const std::string &_z, const std::string &_id = "Vec") :
		m_v(Vec4f(_x, _y, _z, "1.0")),
		m_id(_id)
  {}

  NodeDataType type() const override
  {
		return NodeDataType {"Vector", m_id.c_str(), Qt::yellow};
  }

  Vec4f vector() const
  {
    return m_v;
  }

private:
  Vec4f m_v;
	std::string m_id;
};

class ColorData : public NodeData
{
public:

  ColorData() : m_cd(Vec4f()) {}
  ColorData(const std::string &_r, const std::string &_g, const std::string &_b) :
    m_cd(Vec4f(_r, _g, _b, "1.0")) {}

  NodeDataType type() const override
  {
    return NodeDataType {"Color", "Cd=", Qt::white};
  }

  Vec4f color() const
  {
    return m_cd;
  }
private:
  Vec4f m_cd;
};

class GenericData : public NodeData
{
public:
  GenericData() {}
  NodeDataType type() const override {
    return NodeDataType {"Generic", "Out", Qt::gray};
  }
};

