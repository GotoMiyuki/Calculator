import Foundation

// MARK: - 单项式结构体
struct Term {
    var coef: Int
    var exp: Int
}

// MARK: - 多项式结构体
struct Polynomial {
    private(set) var terms: [Term] = []

    // MARK: 插入项（自动合并 + 排序）
    mutating func insertTerm(_ coef: Int, _ exp: Int) {
        guard coef != 0 else { return }

        if let idx = terms.firstIndex(where: { $0.exp == exp }) {
            terms[idx].coef += coef
            if terms[idx].coef == 0 {
                terms.remove(at: idx)
            }
        } else {
            terms.append(Term(coef: coef, exp: exp))
            terms.sort { $0.exp > $1.exp }
        }
    }

    // MARK: 求导
    func derivative() -> Polynomial {
        var result = Polynomial()
        for t in terms where t.exp > 0 {
            result.insertTerm(t.coef * t.exp, t.exp - 1)
        }
        return result
    }

    // MARK: 计算 P(x)
    func evaluate(_ x: Double) -> Double {
        terms.reduce(0.0) { sum, t in
            sum + Double(t.coef) * pow(x, Double(t.exp))
        }
    }
    
    // MARK: 是否为空
    var isEmpty: Bool { terms.isEmpty }

    // MARK: 文本输出（模仿 C++ display）
    var description: String {
        guard !terms.isEmpty else { return "0" }

        var result = ""
        var first = true

        for t in terms {
            let coef = t.coef
            let exp = t.exp
            let absCoef = abs(coef)

            // --- 符号 ---
            if !first {
                if coef > 0 { result += "+" }
                else if coef < 0 { result += "-" }
            } else if coef < 0 {
                result += "-"
            }

            // --- 系数 + 变量 ---
            if exp == 0 {
                result += "\(absCoef)"
            } else if absCoef == 1 {
                // ±x or ±x^n
                result += "x" + (exp == 1 ? "" : superscript(exp))
            } else {
                // ±kx or ±kx^n
                result += "\(absCoef)x" + (exp == 1 ? "" : superscript(exp))
            }

            first = false
        }

        return result
    }

    // MARK: 上标格式输出（用于 UI）
    func toSuperscript() -> String {
        if isEmpty { return "0" }

        var text = ""
        for t in terms {
            let sign = t.coef > 0 && !text.isEmpty ? "+" : (t.coef < 0 ? "-" : "")
            let absCoef = abs(t.coef)
            if t.exp == 0 {
                text += "\(sign)\(absCoef)"
            } else if absCoef == 1 {
                text += "\(sign)x" + (t.exp == 1 ? "" : superscript(t.exp))
            } else {
                text += "\(sign)\(absCoef)x" + (t.exp == 1 ? "" : superscript(t.exp))
            }
        }
        return text
    }

    // MARK: 上标数字映射
    private func superscript(_ n: Int) -> String {
        let map: [Character: Character] = [
            "0": "⁰", "1": "¹", "2": "²", "3": "³",
            "4": "⁴", "5": "⁵", "6": "⁶",
            "7": "⁷", "8": "⁸", "9": "⁹"
        ]
        return String(String(n).compactMap { map[$0] })
    }

    // MARK: 运算符重载（加减乘）
    static func + (lhs: Polynomial, rhs: Polynomial) -> Polynomial {
        var result = lhs
        for t in rhs.terms {
            result.insertTerm(t.coef, t.exp)
        }
        return result
    }

    static func - (lhs: Polynomial, rhs: Polynomial) -> Polynomial {
        var result = lhs
        for t in rhs.terms {
            result.insertTerm(-t.coef, t.exp)
        }
        return result
    }

    static func * (lhs: Polynomial, rhs: Polynomial) -> Polynomial {
        var result = Polynomial()
        for t1 in lhs.terms {
            for t2 in rhs.terms {
                result.insertTerm(t1.coef * t2.coef, t1.exp + t2.exp)
            }
        }
        return result
    }
}

// MARK: - 字符串解析扩展
extension Polynomial {
    static func parse(_ s: String) -> Polynomial {
        var p = Polynomial()
        let clean = s.replacingOccurrences(of: " ", with: "")
        
        // 改进正则：匹配类似 "3x^2", "-x", "+5" 等项，防止空串匹配
        let pattern = #"([+-]?\d*(?:x(?:\^?\d+)?)?)"#
        let regex = try! NSRegularExpression(pattern: pattern)
        
        for match in regex.matches(in: clean, range: NSRange(clean.startIndex..., in: clean)) {
            guard let range = Range(match.range(at: 1), in: clean) else { continue }
            let term = String(clean[range])
            if term.isEmpty { continue }

            var coef = 0
            var exp = 0

            if term.contains("x") {
                // --- 有变量 ---
                let parts = term.split(separator: "x", omittingEmptySubsequences: false)
                let coefStr = parts.first.map(String.init) ?? ""
                let expStr = parts.count > 1 && parts[1].hasPrefix("^")
                    ? String(parts[1].dropFirst())
                    : (parts.count > 1 ? "1" : "1")

                if coefStr.isEmpty || coefStr == "+" {
                    coef = 1
                } else if coefStr == "-" {
                    coef = -1
                } else {
                    coef = Int(coefStr) ?? 0
                }
                exp = Int(expStr) ?? 1
            } else {
                // --- 常数项 ---
                coef = Int(term) ?? 0
                exp = 0
            }

            if coef != 0 {
                p.insertTerm(coef, exp)
            }
        }

        return p
    }
}
