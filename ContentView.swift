import SwiftUI
import UIKit   // 用于 Haptics

// MARK: - ContentView 主界面
struct ContentView: View {
    @State private var exprInput = ""
    @State private var exprResult = ""
    
    @State private var p1Input = ""
    @State private var p2Input = ""
    @State private var polyResult = ""
    
    @State private var activeField: Int = 1 // 1 表示 P₁，2 表示 P₂
    @State private var showXInput = false
    @State private var xValue = ""
    @State private var evalResult = ""
    
    /// 求导操作
    func performDerivative() {
        let targetPoly = activeField == 1 ? Polynomial.parse(p1Input) : Polynomial.parse(p2Input)
        let result = targetPoly.derivative()
        withAnimation(.easeInOut) {
            polyResult = result.description
        }
        let impact = UIImpactFeedbackGenerator(style: .medium)
        impact.impactOccurred()
    }
    
    /// 弹出输入框计算 P(x)
    func showEvaluateAlert() {
        let impact = UIImpactFeedbackGenerator(style: .light)
        impact.impactOccurred()
        showXInput = true
    }
    
    /// 实际计算 P(x)
    func performEvaluate() {
        guard let x = Double(xValue) else {
            polyResult = "⚠️ Invalid x value"
            return
        }
        let targetPoly = activeField == 1 ? Polynomial.parse(p1Input) : Polynomial.parse(p2Input)
        let result = targetPoly.evaluate(x)
        withAnimation(.easeInOut) {
            polyResult = "P(\(x)) = \(result)"
        }
        let impact = UIImpactFeedbackGenerator(style: .rigid)
        impact.impactOccurred()
    }
    
    var body: some View {
        TabView {
            // MARK: - Polynomial Calculator
            ScrollView {
                VStack(spacing: 15) {
                    Spacer(minLength: 10)
                    // --- P1 输入区
                    PolyInputField(title: "Polynomial 1", text: $p1Input, active: $activeField, id: 1)
                    
                    // --- P2 输入区
                    PolyInputField(title: "Polynomial 2", text: $p2Input, active: $activeField, id: 2)
                    
                    // --- 操作按钮
                    VStack(spacing: 10) {
                        HStack(spacing: 10) {
                            Button("P₁ + P₂") { performOp("+") }
                                .buttonStyle(.borderedProminent)
                            Button("P₁ - P₂") { performOp("-") }
                                .buttonStyle(.borderedProminent)
                            Button("P₁ × P₂") { performOp("*") }
                                .buttonStyle(.borderedProminent)
                        }
                        
                        HStack(spacing: 10) {
                            Button("Derivative") { performDerivative() }
                                .buttonStyle(.bordered)
                            Button("Evaluate P(x)") { showEvaluateAlert() }
                                .buttonStyle(.bordered)
                        }
                    }
                    
                    // --- 当前操作目标提示
                    Text(activeField == 1 ? "🔹 Operating on P₁" : "🔸 Operating on P₂")
                        .foregroundColor(.gray)
                        .font(.footnote)
                    
                    // --- 输出结果（上标格式）
                    VStack {
                        Text("Result:")
                            .foregroundStyle(.blue)
                        Text(polyResult.isEmpty ? "-" : polyResult)
                            .foregroundColor(.blue)
                            .font(.title3)
                            .multilineTextAlignment(.center)
                            .padding(8)
                            .background(Color(.systemGray6))
                            .cornerRadius(10)
                            .padding(.horizontal)
                    }
                    
                    // --- 统一输入键盘
                    CalculatorPad(input1: $p1Input, input2: $p2Input, activeField: $activeField)
                    
                }
            }
            .tabItem {
                Label("Polynomial", systemImage: "function")
            }
            
            // MARK: - Expression Calculator
            VStack(spacing: 25) {
                Spacer(minLength: 40)
                Text(exprInput.isEmpty ? "Enter Expression:" : Expression(exprInput).prettyFormat())
                    .font(.title3)
                    .frame(maxWidth: .infinity, minHeight: 60)
                    .background(Color(.systemGray6))
                    .cornerRadius(12)
                    .padding(.horizontal)
                
                Text("Result:")
                    .foregroundColor(.blue)
                Text(exprResult.isEmpty ? "-" : exprResult)
                    .foregroundColor(.blue)
                    .font(.title3)
                    .multilineTextAlignment(.center)
                    .padding(8)
                    .background(Color(.systemGray6))
                    .cornerRadius(10)
                    .padding(.horizontal)
                
                CalculatorPadExpr(input: $exprInput, result: $exprResult)
                
                Spacer()
            }
            .tabItem {
                Label("Expression", systemImage: "plus.slash.minus")
            }.alert("Evaluate P(x)", isPresented: $showXInput) {
                TextField("Enter x value", text: $xValue)
                    .keyboardType(.decimalPad)
                Button("OK", action: performEvaluate)
                Button("Cancel", role: .cancel) {}
            }
        }
        .tint(.gray)
        .accentColor(.blue)
    }
    
    // MARK: - Polynomial 运算逻辑
    func performOp(_ op: String) {
        let p1 = Polynomial.parse(p1Input)
        let p2 = Polynomial.parse(p2Input)
        var result: Polynomial
        
        switch op {
        case "+": result = p1 + p2
        case "-": result = p1 - p2
        case "*": result = p1 * p2
        default: return
        }
        
        polyResult = result.toSuperscript() // 上标输出
        Haptics.shared.impact()
    }
    
    // MARK: - 多项式输入框组件
    struct PolyInputField: View {
        var title: String
        @Binding var text: String
        @Binding var active: Int
        var id: Int
        
        var body: some View {
            VStack {
                Text(text.isEmpty ? "Tap to enter \(title)" : text)
                    .font(.title3)
                    .frame(maxWidth: .infinity, alignment: .center)
                    .padding(.vertical, 10)
                    .background(Color(.systemGray6))
                    .cornerRadius(12)
                    .overlay(
                        RoundedRectangle(cornerRadius: 12)
                            .inset(by: 1)
                            .stroke(active == id ? Color.blue : Color.clear, lineWidth: 2)
                    )
                    .onTapGesture {
                        active = id
                        Haptics.shared.selection()
                    }
            }
            .padding(.horizontal, 16)
        }
    }
    
    // MARK: - 操作按钮样式
    struct CalcOpButton: View {
        var title: String
        var action: () -> Void
        @State private var pressed = false
        
        var body: some View {
            Button(action: {
                withAnimation(.easeInOut(duration: 0.15)) {
                    pressed = true
                    Haptics.shared.impact()
                }
                DispatchQueue.main.asyncAfter(deadline: .now() + 0.15) {
                    pressed = false
                    action()
                }
            }) {
                Text(title)
                    .font(.headline)
                    .frame(width: 100, height: 45)
                    .background(pressed ? Color.blue.opacity(0.3) : Color.blue.opacity(0.15))
                    .foregroundColor(.blue)
                    .cornerRadius(10)
                    .scaleEffect(pressed ? 0.93 : 1.0)
            }
        }
    }
    
    // MARK: - 通用键盘（多项式输入）
    struct CalculatorPad: View {
        @Binding var input1: String
        @Binding var input2: String
        @Binding var activeField: Int
        
        let buttons: [[String]] = [
            ["C", "^", "←", "+"],
            ["7", "8", "9", "-"],
            ["4", "5", "6", "*"],
            ["1", "2", "3", "/"],
            ["0", "(", ")", "x"]
        ]
        
        var body: some View {
            VStack(spacing: 12) {
                ForEach(buttons, id: \.self) { row in
                    HStack(spacing: 12) {
                        ForEach(row, id: \.self) { label in
                            CalcKey(label: label) { handleButton(label) }
                        }
                    }
                }
            }
            .padding(.horizontal)
        }
        
        func handleButton(_ label: String) {
            var target: Binding<String> {
                activeField == 1 ? $input1 : $input2
            }
            Haptics.shared.selection()
            
            switch label {
            case "C": target.wrappedValue = ""
            case "←":
                if !target.wrappedValue.isEmpty { target.wrappedValue.removeLast() }
            case "×": target.wrappedValue.append("*")
            case "÷": target.wrappedValue.append("/")
            default: target.wrappedValue.append(label)
            }
        }
    }
    
    // MARK: - 按钮组件（动画+Haptic）
    struct CalcKey: View {
        var label: String
        var action: () -> Void
        @State private var pressed = false
        
        var body: some View {
            Button(action: {
                withAnimation(.easeInOut(duration: 0.1)) {
                    pressed = true
                    Haptics.shared.lightImpact()
                }
                DispatchQueue.main.asyncAfter(deadline: .now() + 0.1) {
                    pressed = false
                    action()
                }
            }) {
                Text(label)
                    .font(.title2)
                    .frame(width: 70, height: 50)
                    .background(pressed ? Color.blue.opacity(0.25) : Color.blue.opacity(0.12))
                    .foregroundColor(.blue)
                    .cornerRadius(10)
                    .scaleEffect(pressed ? 0.9 : 1.0)
            }
        }
    }
    
    // MARK: - 表达式输入键盘
    struct CalculatorPadExpr: View {
        @Binding var input: String
        @Binding var result: String
        
        let buttons: [[String]] = [
            ["C", "^", "←", "+"],
            ["7", "8", "9", "-"],
            ["4", "5", "6", "*"],
            ["1", "2", "3", "/"],
            ["0", "(", ")", "="]
        ]
        
        var body: some View {
            VStack(spacing: 12) {
                ForEach(buttons, id: \.self) { row in
                    HStack(spacing: 12) {
                        ForEach(row, id: \.self) { label in
                            CalcKey(label: label) { handleButton(label) }
                        }
                    }
                }
            }
            .padding(.horizontal)
        }
        
        func handleButton(_ label: String) {
            Haptics.shared.selection()
            switch label {
            case "C": input = ""
            case "←": if !input.isEmpty { input.removeLast() }
            case "=":
                do {
                    let e = Expression(input)
                    result = String(format: "%.2f", try e.evaluate())
                    Haptics.shared.impact()
                } catch {
                    result = error.localizedDescription
                }
                
            default: input.append(label)
            }
        }
    }
    
    // MARK: - Haptic 管理器
    final class Haptics {
        static let shared = Haptics()
        private let generatorImpact = UIImpactFeedbackGenerator(style: .medium)
        private let generatorLight = UIImpactFeedbackGenerator(style: .light)
        private let generatorSelect = UISelectionFeedbackGenerator()
        
        func impact() { generatorImpact.impactOccurred() }
        func lightImpact() { generatorLight.impactOccurred() }
        func selection() { generatorSelect.selectionChanged() }
    }
}
