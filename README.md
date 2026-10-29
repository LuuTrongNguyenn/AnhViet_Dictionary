# 📚 HỆ THỐNG TỪ ĐIỂN ANH – VIỆT & GỢI Ý TỪ THÔNG MINH (LEXITRIE)

Dự án bài tập lớn môn **Cấu trúc dữ liệu và Giải thuật (CTDL & GT)**, xây dựng ứng dụng từ điển Anh–Việt bằng **C++17**, sử dụng cấu trúc dữ liệu **Trie (Prefix Tree)** kết hợp thuật toán **DFS (Depth-First Search)** để tìm kiếm và gợi ý từ thông minh.

Ứng dụng hỗ trợ giao diện Windows thông qua **Microsoft WebView2**, kết hợp backend C++ để xử lý dữ liệu và nghiệp vụ.

**Mã đề tài: ĐT06**

---

## 👥 1. Danh sách thành viên nhóm

| STT | Họ và tên | Mã số sinh viên |
|:---:|:---|:---:|
| 1 | Châu Thiện An | 6651071002 |
| 2 | Lưu Trọng Nguyên | 6651071048 |
| 3 | Dương Đình Khánh | 6651071033 |

---

## 📁 2. Cấu trúc thư mục dự án

```text
LexiTrie_WebView2_Login/
│
├── include/
│   ├── Trie.h
│   ├── FileManager.h
│   ├── ConsoleUI.h
│   └── SmartReview.h
│
├── src/
│   ├── Trie.cpp
│   ├── FileManager.cpp
│   ├── ConsoleUI.cpp
│   ├── SmartReview.cpp
│   ├── WebServer.cpp
│   ├── Win32GUI.cpp
│   └── main.cpp
│
├── data/
│   ├── dictionary.txt
│   ├── word_parts.txt
│   └── word_topics.txt
│
├── web/
│   └── (Giao diện HTML, CSS, JavaScript)
│
├── WebViewHost/
│   └── (Ứng dụng Windows sử dụng WebView2)
│
├── tests/
│   └── test_trie.cpp
│
├── benchmarks/
│   └── benchmark.cpp
│
├── docs/
│   └── report.tex
│
├── build_webview2.bat
│
└── README.md
```

*Lưu ý: Sơ đồ trên thể hiện các thành phần chính của dự án.*

---

## 🎯 3. Mục tiêu dự án

- Áp dụng cấu trúc dữ liệu Trie vào bài toán quản lý từ điển Anh–Việt.
- Tìm kiếm từ theo độ dài chuỗi đầu vào.
- Cài đặt thuật toán DFS để gợi ý từ theo tiền tố.
- Hỗ trợ thêm, sửa, xóa và tra cứu từ vựng.
- Xây dựng hệ thống học và ôn tập từ vựng Smart Review.
- Tạo ứng dụng có giao diện trực quan trên Windows.
- Áp dụng GitHub và Jira để quản lý mã nguồn và công việc nhóm.

---

## ⚙️ 4. Các chức năng chính

### 4.1. Quản lý từ điển

| Chức năng | Mô tả |
|---|---|
| Tra cứu từ | Tìm từ tiếng Anh và hiển thị nghĩa tiếng Việt |
| Thêm từ | Bổ sung từ mới vào Trie |
| Cập nhật từ | Chỉnh sửa nghĩa của từ đã tồn tại |
| Xóa từ | Xóa từ khỏi từ điển |
| Gợi ý từ | Đề xuất các từ có cùng tiền tố |
| Tìm kiếm theo từng phím | Cập nhật danh sách gợi ý khi nhập ký tự |
| Lưu dữ liệu | Ghi các thay đổi vào file từ điển |

### 4.2. Smart Review

- Quản lý danh sách từ đã học.
- Lưu lịch sử ôn tập.
- Theo dõi tiến độ học từ vựng.
- Thống kê số lần ôn tập và hoạt động học tập.
- Gợi ý ôn lại các từ đã học.

### 4.3. Giao diện ứng dụng

- Giao diện Dark Mode.
- Trang tra cứu từ vựng.
- Danh sách gợi ý từ thông minh.
- Đăng ký và đăng nhập tài khoản cục bộ.
- Trang cá nhân và lịch sử học tập.
- Giao diện Smart Review và thống kê.
- Kết nối giao diện với backend C++ thông qua API.

---

## 🧠 5. Cấu trúc dữ liệu và thuật toán

### 5.1. Trie (Prefix Tree)

Trie là cấu trúc dữ liệu dạng cây, trong đó mỗi nút biểu diễn một ký tự của từ.

Cấu trúc này giúp tìm kiếm và xử lý tiền tố hiệu quả.

Các thao tác chính:

- `insert()`: Thêm từ vào Trie.
- `search()`: Tìm kiếm từ.
- `update()`: Cập nhật nghĩa.
- `remove()`: Xóa từ.
- Autocomplete: Gợi ý các từ có cùng tiền tố.

### 5.2. DFS (Depth-First Search)

Thuật toán DFS được sử dụng để duyệt các nhánh của Trie và thu thập các từ có cùng tiền tố.

Ví dụ, khi nhập `app`, chương trình có thể gợi ý:

- apple
- application
- apply

### 5.3. Phân tích độ phức tạp

| Thuật toán | Độ phức tạp |
|---|---|
| Thêm từ | O(L) |
| Tìm kiếm | O(L) |
| Cập nhật nghĩa | O(L) |
| Xóa từ | O(L) |
| Gợi ý theo tiền tố | O(P + V) |

Trong đó:

- `L`: Độ dài từ.
- `P`: Độ dài tiền tố.
- `V`: Số nút được duyệt trong quá trình thu thập gợi ý.

---

## 💻 6. Công nghệ sử dụng

| Công nghệ | Mục đích |
|---|---|
| C++17 | Xử lý thuật toán và nghiệp vụ |
| Trie | Lưu trữ, tìm kiếm từ vựng |
| DFS | Gợi ý từ theo tiền tố |
| UTF-8 | Lưu dữ liệu Anh–Việt |
| HTML, CSS, JavaScript | Xây dựng giao diện |
| Microsoft WebView2 | Hiển thị giao diện trên Windows |
| .NET | Chạy WebView2 Host |
| Git & GitHub | Quản lý mã nguồn |
| Jira | Quản lý tiến độ và phân công |
| LaTeX | Viết báo cáo đồ án |

---

## 🚀 7. Hướng dẫn cài đặt và chạy chương trình

### 7.1. Yêu cầu hệ thống

- Windows 10 hoặc Windows 11.
- Trình biên dịch C++ hỗ trợ chuẩn C++17.
- MSYS2 MinGW-w64 hoặc trình biên dịch tương thích.
- .NET SDK phù hợp với dự án WebView2 Host.
- Microsoft Edge WebView2 Runtime.

### 7.2. Tải mã nguồn

```bash
git clone <URL_REPOSITORY_LEXITRIE>
cd LexiTrie_WebView2_Login
```

Thay `<URL_REPOSITORY_LEXITRIE>` bằng đường dẫn GitHub thực tế của nhóm.

### 7.3. Chạy ứng dụng WebView2

Mở PowerShell tại thư mục gốc dự án:

```powershell
.\build_webview2.bat
```

Sau khi build thành công, khởi chạy ứng dụng theo hướng dẫn của script.

### 7.4. Biên dịch phiên bản Console

Sử dụng trình biên dịch `g++`:

```powershell
g++ -std=c++17 -Iinclude src/main.cpp src/Trie.cpp src/FileManager.cpp src/ConsoleUI.cpp src/SmartReview.cpp -o LexitrieConsole.exe
```

Chạy chương trình:

```powershell
.\LexitrieConsole.exe
```

**Lưu ý:** Cần chạy chương trình tại thư mục gốc để các đường dẫn dữ liệu tương đối hoạt động đúng.

---

## 👨‍💻 8. Phân công công việc

### 8.1. Lưu Trọng Nguyên – Trie và thuật toán

**File phụ trách:**
- `include/Trie.h`
- `src/Trie.cpp`

**Công việc:**
- Thiết kế TrieNode và lớp Trie.
- Cài đặt thêm, tìm kiếm, cập nhật, xóa từ.
- Cài đặt DFS Autocomplete.
- Xử lý giải phóng bộ nhớ Trie.
- Viết báo cáo cấu trúc dữ liệu và giải thuật.

### 8.2. Châu Thiện An – Console và Backend

**File phụ trách:**
- `include/ConsoleUI.h`
- `src/ConsoleUI.cpp`
- `src/main.cpp`
- `src/WebServer.cpp`

**Công việc:**
- Xây dựng menu Console.
- Cài đặt gợi ý theo từng phím.
- Khởi tạo chương trình và kết nối các lớp C++.
- Xây dựng backend API.
- Phụ trách tích hợp giao diện WebView2.
- Viết báo cáo phân tích yêu cầu và cài đặt chương trình.

### 8.3. Dương Đình Khánh – Dữ liệu và Smart Review

**File phụ trách:**
- `include/FileManager.h`
- `src/FileManager.cpp`
- `include/SmartReview.h`
- `src/SmartReview.cpp`
- `data/`

**Công việc:**
- Thu thập và chuẩn hóa dữ liệu Anh–Việt.
- Xây dựng chức năng đọc/ghi file.
- Xây dựng Smart Review.
- Quản lý tiến độ học tập và thống kê.
- Viết báo cáo thực nghiệm và kết luận.

---

## 📊 9. Quản lý dự án

### GitHub

- Mỗi thành viên sử dụng tài khoản GitHub riêng.
- Ph
