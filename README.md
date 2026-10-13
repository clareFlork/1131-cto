# 1131-ctomips
一個以 C++ 撰寫的簡易 MIPS 組譯器，可將 MIPS 組合語言指令轉換為 32 位元二進位 或 8 位 16 進位 機器碼。
# MIPS Assembler (Mini)


## 功能特色

- 讀取 `.asm` 組合語言檔案，逐行解析
- 自動忽略空行、組譯指示（以 `.` 開頭，如 `.text`、`.data`）
- 支援 label 移除（例如 `loop: add $t0, $t1, $t2`）
- 支援輸出格式切換：二進位 (`2`) 或 16 進位 (`16`)
- 支援 `$r0`、`$at`、`$v0`~`$v1`、`$a0`~`$a3`、`$t0`~`$t9`、`$s0`~`$s8`、`$k0`~`$k1`、`$gp`、`$sp`、`$ra` 暫存器名稱

## 支援的指令

| 指令 | 格式 | Opcode / Funct | 範例 |
|------|------|----------------|------|
| `add`  | R-format | op=`000000`, funct=`100000` | `add $t0, $t1, $t2` |
| `sub`  | R-format | op=`000000`, funct=`100010` | `sub $t0, $t1, $t2` |
| `addi` | I-format | op=`001000` | `addi $t0, $t1, 10` |
| `slti` | I-format | op=`001010` | `slti $t0, $t1, 5` |

### 指令格式

```
R-format: op(6) | rs(5) | rt(5) | rd(5) | shamt(5) | funct(6)
I-format: op(6) | rs(5) | rt(5) | immediate(16)
```

## 編譯

```bash
g++ -std=c++11 -o assembler main.cpp
```

> 若使用 Visual Studio，請建立 Console 專案並於「偵錯 → 命令引數」中設定參數。

## 使用方式

```bash
./assembler <輸入檔> <輸出進位>
```

| 參數 | 說明 |
|------|------|
| `argv[1]` | 組合語言檔案路徑 |
| `argv[2]` | `2` 輸出二進位；`16` 輸出 16 進位 |

### 範例

輸入檔 `test.asm`：

```asm
.text
main:   addi $t0, $zero, 10
        add  $t1, $t0, $t0
        sub  $t2, $t1, $t0
        slti $t3, $t2, 5
```

> 注意：目前暫存器表以 `$r0` 表示 0 號暫存器，請在輸入檔中使用 `$r0` 而非 `$zero`。

執行：

```bash
./assembler test.asm 2
```

輸出（二進位）：

```
I-format:00100000000010000000000000001010
R-format:00000001000010000100100000100000
R-format:00000001001010000101000000100010
i-format:00101001010010110000000000000101
```

執行：

```bash
./assembler test.asm 16
```

輸出（16 進位）：

```
I-format:2008000a
R-format:01084820
R-format:01285022
i-format:294b0005
```

## 運作流程

1. 以 `ifstream` 開啟 `argv[1]` 指定的檔案
2. 逐行讀取，使用 `istringstream` 切割單詞
3. 移除 label（`:` 之前的內容）並以逗號切割運算元
4. 跳過以 `.` 開頭的組譯指示
5. 將結果存入 `argu[]`：`argu[0]` 為指令，`argu[1..3]` 為運算元
6. 依指令類型查詢暫存器編號、組合二進位字串
7. 依 `argv[2]` 決定輸出二進位或 16 進位

## 已知限制 / 待改進

- 目前僅支援 `add`、`sub`、`addi`、`slti` 四個指令
- 暫存器表中 `a3` 缺少 `$` 前綴（應為 `$a3`），且 0 號暫存器以 `$r0` 表示，不支援 `$zero`
- 未實作 `lw`、`sw`、`beq`、`j` 等指令與 label 位址計算
- 未檢查參數數量（缺少 `argv[2]` 時可能發生錯誤）
- 輸出前綴大小寫不一致（`I-format` / `i-format`）
- 註解（`#`）尚未處理

