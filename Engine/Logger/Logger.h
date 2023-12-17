#pragma once
#include<string>
#include <Windows.h>
#include<format>

void Log(const std::string& message);

/// <summary>
/// stringからwstringに変換する関数
/// </summary>
/// <param name="str"><文字列/param>
/// <returns>変換後の文字列</returns>
std::wstring ConvertString(const std::string& str);

/// <summary>
///  wstringからstringに変換する関数
/// </summary>
/// <param name="str">文字列</param>
/// <returns>変換後の文字列</returns>
std::string ConvertString(const std::wstring& str);