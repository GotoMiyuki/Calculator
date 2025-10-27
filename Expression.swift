import Foundation

class Expression {
    private var infix: String

    init(_ str: String) { self.infix = str }

    private func priority(_ p: Character, _ q: Character) -> Bool {
        if p == "(" { return false }
        if p == "^" && q == "^" { return false }
        if p == "^" { return true }
        if p == "*" || p == "/" { return true }
        if (p == "+" || p == "-") && (q == "+" || q == "-") { return true }
        return false
    }

    private func infixToPostfix() throws -> [String] {
        var output: [String] = []
        var stack: [Character] = []
        let chars = Array(infix)

        var i = 0
        while i < chars.count {
            let c = chars[i]
            if c.isWhitespace { i += 1; continue }

            if c.isNumber {
                var num = ""
                while i < chars.count, chars[i].isNumber {
                    num.append(chars[i])
                    i += 1
                }
                output.append(num)
                continue
            } else if c == "(" {
                stack.append(c)
            } else if c == ")" {
                while let top = stack.last, top != "(" {
                    output.append(String(stack.removeLast()))
                }
                if stack.isEmpty { throw NSError(domain: "Expression", code: 1, userInfo: [NSLocalizedDescriptionKey: "Unmatched ')'"]) }
                stack.removeLast()
            } else if "+-*/^".contains(c) {
                while let top = stack.last, top != "(", priority(top, c) {
                    output.append(String(stack.removeLast()))
                }
                stack.append(c)
            } else {
                throw NSError(domain: "Expression", code: 2, userInfo: [NSLocalizedDescriptionKey: "Invalid character: \(c)"])
            }
            i += 1
        }

        while !stack.isEmpty {
            let top = stack.removeLast()
            if top != "(" { output.append(String(top)) }
        }

        return output
    }

    func evaluate() throws -> Double {
        let postfix = try infixToPostfix()
        var stack: [Double] = []

        for token in postfix {
            if let num = Double(token) {
                stack.append(num)
            } else if "+-*/^".contains(token) {
                guard stack.count >= 2 else {
                    throw NSError(domain: "Expression", code: 3, userInfo: [NSLocalizedDescriptionKey: "Insufficient operands"])
                }
                let b = stack.removeLast()
                let a = stack.removeLast()
                var res = 0.0
                switch token {
                case "+": res = a + b
                case "-": res = a - b
                case "*": res = a * b
                case "/":
                    guard b != 0 else {
                        throw NSError(domain: "Expression", code: 4, userInfo: [NSLocalizedDescriptionKey: "Division by zero"])
                    }
                    res = a / b
                case "^": res = pow(a, b)
                default: break
                }
                stack.append(res)
            }
        }

        guard stack.count == 1 else {
            throw NSError(domain: "Expression", code: 5, userInfo: [NSLocalizedDescriptionKey: "Invalid expression"])
        }
        return stack[0]
    }
}
extension Expression {
    func prettyFormat() -> String {
        // 将 ^n 替换为上标字符
        let superscriptMap: [Character: Character] = [
            "0": "⁰", "1": "¹", "2": "²", "3": "³", "4": "⁴",
            "5": "⁵", "6": "⁶", "7": "⁷", "8": "⁸", "9": "⁹"
        ]
        
        var output = ""
        var i = infix.startIndex
        while i < infix.endIndex {
            let c = infix[i]
            if c == "^" {
                i = infix.index(after: i)
                while i < infix.endIndex, let mapped = superscriptMap[infix[i]] {
                    output.append(mapped)
                    i = infix.index(after: i)
                }
                continue
            } else {
                output.append(c)
            }
            i = infix.index(after: i)
        }
        return output
    }
}
