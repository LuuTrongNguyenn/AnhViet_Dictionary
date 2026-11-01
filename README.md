# 📚 HỆ THỐNG TỪ ĐIỂN ANH – VIỆT & GỢI Ý TỪ THÔNG MINH (LEXITRIE)

Dự án Bài tập lớn môn **Cấu trúc dữ liệu và Giải thuật (CTDL & GT)** xây dựng ứng dụng từ điển Anh–Việt và hỗ trợ học từ vựng trên nền tảng **C++17**, sử dụng cấu trúc dữ liệu **Trie (Prefix Tree)** kết hợp thuật toán **DFS (Depth-First Search)** để tối ưu tìm kiếm và gợi ý từ theo tiền tố.

Hệ thống được phát triển với giao diện **Dark Mode hiện đại bằng Microsoft WebView2**, kết hợp HTML, CSS, JavaScript và backend C++ để xử lý các chức năng tra cứu, quản lý từ vựng, đăng nhập tài khoản và ôn tập thông minh.

**Mã đề tài: ĐT06**

---

## 👥 Danh sách thành viên nhóm

| STT | Họ và tên | Mã số sinh viên |
|:---:|:---|:---:|
| 1 | Châu Thiện An | 6651071002 |
| 2 | Lưu Trọng Nguyên | 6651071048 |
| 3 | Dương Đình Khánh | 6651071033 |

---

## 📁 Cấu trúc thư mục dự án

```text
LexiTrie_WebView2_Login/
│
├── README.md                       # Giới thiệu dự án, chức năng và hướng dẫn chạy
├── build_webview2.bat              # Script biên dịch và khởi chạy ứng dụng WebView2
│
├── data/                           # Dữ liệu từ điển và thông tin bổ sung
│   ├── dictionary.txt              # Dữ liệu từ điển Anh–Việt UTF-8
│   ├── word_parts.txt              # Dữ liệu từ loại
│   └── word_topics.txt             # Dữ liệu chủ đề từ vựng
│
├── include/                        # Khai báo lớp và cấu trúc dữ liệu C++
│   ├── Trie.h                      # TrieNode, Trie và các phương thức quản lý từ
│   ├── FileManager.h               # Khai báo chức năng đọc và ghi file từ điển
│   ├── ConsoleUI.h                 # Khai báo giao diện Console
│   └── SmartReview.h               # Khai báo hệ thống ôn tập từ vựng
│
├── src/                            # Triển khai mã nguồn C++
│   ├── Trie.cpp                    # Thêm, tìm kiếm, cập nhật, xóa và DFS Autocomplete
│   ├── FileManager.cpp             # Đọc, nạp, ghi và lưu dữ liệu từ điển
│   ├── ConsoleUI.cpp               # Menu và các thao tác từ điển trên Console
│   ├── SmartReview.cpp             # Quản lý ôn tập, lịch sử và thống kê
│   ├── WebServer.cpp               # Backend API kết nối C++ với giao diện
│   └── main.cpp                    # Điểm khởi chạy chương trình Console
│
├── web/                            # Giao diện ứng dụng
│   ├── index.html                  # Giao diện chính
│   └── (Các file CSS, JavaScript và tài nguyên giao diện)
│
├── WebViewHost/                    # Ứng dụng Windows sử dụng Microsoft WebView2
│   ├── Program.cs                  # Điểm khởi chạy WebView2 Host
│   └── WebViewHost.csproj          # Cấu hình dự án .NET
│
├── tests/                          # Kiểm tra chức năng cấu trúc dữ liệu
│   └── test_trie.cpp               # Kiểm tra các thao tác Trie
│
├── benchmarks/                     # Đánh giá hiệu năng
│   └── benchmark.cpp               # Đo hiệu năng các thao tác từ điển
│
└── docs/                           # Tài liệu báo cáo
    └── report.tex                  # Báo cáo đồ án bằng LaTeX
```

*Lưu ý: Cấu trúc trên thể hiện các thành phần chính của phiên bản WebView2. Các file giao diện Win32 cũ và file sinh ra khi biên dịch không được liệt kê.*

---

## 🎯 Các chức năng chính của chương trình

Ứng dụng LexiTrie cung cấp hệ thống từ điển Anh–Việt kết hợp công cụ học và ôn tập từ vựng.

### 1. Tra cứu từ điển Anh–Việt

- Cho phép người dùng nhập từ tiếng Anh cần tra cứu.
- Tìm kiếm từ bằng cấu trúc dữ liệu Trie.
- Hiển thị nghĩa tiếng Việt của từ được tìm thấy.
- Thông báo khi từ không tồn tại trong từ điển.
- Hỗ trợ tra cứu trên giao diện WebView2 và Console.

### 2. Gợi ý từ thông minh (Autocomplete)

- Hiển thị danh sách từ gợi ý theo tiền tố người dùng nhập.
- Cập nhật gợi ý khi người dùng nhập thêm ký tự.
- Sử dụng Trie để tìm nhanh nút tương ứng với tiền tố.
- Áp dụng DFS để thu thập những từ hợp lệ trong các nhánh con.
- Giúp giảm thời gian nhập và hỗ trợ tìm kiếm từ vựng.

**Ví dụ:**

Khi nhập `app`, hệ thống có thể đề xuất:

- `apple`
- `application`
- `apply`

### 3. Quản lý dữ liệu từ điển

- **Thêm từ:** Bổ sung từ mới và nghĩa tiếng Việt vào Trie.
- **Cập nhật từ:** Chỉnh sửa nghĩa của từ đã tồn tại.
- **Xóa từ:** Loại bỏ từ khỏi cấu trúc Trie.
- **Đọc dữ liệu:** Nạp dữ liệu từ file từ điển UTF-8.
- **Lưu dữ liệu:** Ghi những thay đổi xuống file.
- **Từ loại và chủ đề:** Sử dụng dữ liệu bổ sung phục vụ tra cứu và học tập.

### 4. Đăng ký và đăng nhập tài khoản

- Cho phép tạo tài khoản người dùng cục bộ.
- Hỗ trợ đăng nhập để sử dụng các chức năng cá nhân.
- Cung cấp giao diện quản lý thông tin tài khoản.
- Kết hợp tài khoản với các chức năng học từ vựng.

### 5. Hệ thống Smart Review

- Quản lý danh sách từ đã học.
- Theo dõi các từ cần ôn tập.
- Lưu kết quả và lịch sử ôn tập.
- Thống kê hoạt động học tập.
- Theo dõi tiến độ ghi nhớ từ vựng.
- Hỗ trợ gợi ý ôn lại những từ đã học.

### 6. Thống kê và lịch sử học tập

- Hiển thị lịch sử tra cứu từ vựng.
- Theo dõi số lần ôn tập.
- Tổng hợp kết quả học từ vựng.
- Hiển thị các chỉ số thống kê trên giao diện.
- Hỗ trợ người dùng đánh giá tiến độ học tập.

### 7. Giao diện Dark Mode bằng WebView2

- Thiết kế giao diện hiện đại, dễ sử dụng.
- Trang chủ và khu vực tra cứu từ điển.
- Danh sách gợi ý từ khi nhập.
- Giao diện đăng ký, đăng nhập và trang cá nhân.
- Khu vực Smart Review.
- Biểu đồ và thống kê học tập.
- Kết nối với backend C++ thông qua API.

---

## 💡 Thiết kế cấu trúc dữ liệu và thuật toán

### 1. Trie (Prefix Tree)

Trie là cấu trúc dữ liệu dạng cây được sử dụng để lưu trữ các từ theo từng ký tự.

Mỗi nút đại diện cho một ký tự, các từ có chung tiền tố sẽ chia sẻ các nút tương ứng.

**Các thành phần chính:**

- `TrieNode`: Biểu diễn một nút trong cây Trie.
- `Trie`: Quản lý nút gốc và các thao tác trên cây.
- `insert()`: Thêm từ mới.
- `search()`: Tìm kiếm từ.
- `update()`: Cập nhật nghĩa.
- `remove()`: Xóa từ.
- DFS Autocomplete: Thu thập các từ theo tiền tố.

**Ưu điểm:**

- Tìm kiếm hiệu quả theo độ dài từ.
- Tận dụng tiền tố chung để lưu trữ.
- Phù hợp với chức năng gợi ý từ tự động.

### 2. DFS (Depth-First Search)

DFS là thuật toán duyệt theo chiều sâu được áp dụng để tìm các từ có chung tiền tố trong Trie.

**Quy trình:**

1. Nhận tiền tố từ người dùng.
2. Tìm nút Trie tương ứng với tiền tố.
3. Duyệt các nhánh con bằng DFS.
4. Thu thập những nút đánh dấu kết thúc từ.
5. Trả về danh sách từ gợi ý.

### 3. Quản lý bộ nhớ

- Các nút Trie được cấp phát động.
- Khi xóa từ, chương trình cần bảo toàn các nhánh vẫn được từ khác sử dụng.
- Khi hủy Trie, các nút được giải phóng để hạn chế rò rỉ bộ nhớ.
- Các thao tác thêm, xóa và giải phóng bộ nhớ được triển khai trong lớp Trie.

### 4. Đọc và ghi dữ liệu

- Dữ liệu từ điển được lưu dưới dạng văn bản UTF-8.
- FileManager đảm nhiệm đọc, nạp và lưu dữ liệu.
- Trie quản lý các từ đã được nạp trong bộ nhớ.
- Các thao tác chỉnh sửa từ điển được kết hợp với cơ chế lưu trữ dữ liệu.

### 5. Phân tích độ phức tạp

| Thuật toán | Độ phức tạp |
|---|---|
| Thêm từ | O(L) |
| Tìm kiếm từ | O(L) |
| Cập nhật nghĩa | O(L) |
| Xóa từ | O(L) |
| Gợi ý theo tiền tố | O(P + V) |

Trong đó:

- `L`: Độ dài từ cần xử lý.
- `P`: Độ dài tiền tố.
- `V`: Số nút được duyệt trong quá trình DFS.

Độ phức tạp trên xét theo mô hình Trie có bảng chữ cái cố định.

---

## 🌐 Kiến trúc ứng dụng WebView2

LexiTrie sử dụng kiến trúc kết hợp **giao diện web và backend C++**.

### 1. Giao diện Frontend

Được xây dựng bằng:

- HTML: Cấu trúc giao diện.
- CSS: Thiết kế Dark Mode.
- JavaScript: Xử lý sự kiện và tương tác người dùng.

### 2. Backend C++

Thành phần `WebServer.cpp` cung cấp các API phục vụ:

- Tra cứu từ điển.
- Gợi ý từ theo tiền tố.
- Thêm và xóa từ.
- Quản lý dữ liệu học tập.
- Smart Review và thống kê.
- Các chức năng tài khoản cục bộ.

### 3. Microsoft WebView2 Host

- Sử dụng .NET để tạo ứng dụng Windows.
- Hiển thị giao diện HTML, CSS và JavaScript thông qua WebView2.
- Kết nối giao diện với backend C++.
- Cho phép người dùng sử dụng ứng dụng trong cửa sổ Windows riêng.

### 4. Luồng xử lý dữ liệu

```text
Người dùng
    |
    v
Giao diện WebView2
(HTML, CSS, JavaScript)
    |
    v
Backend API C++
(WebServer.cpp)
    |
    +------> Trie
    |        - Tìm kiếm
    |        - Thêm / sửa / xóa
    |        - DFS Autocomplete
    |
    +------> FileManager
    |        - Đọc dữ liệu
    |        - Lưu dữ liệu
    |
    +------> SmartReview
             - Quản lý từ đã học
             - Lịch sử ôn tập
             - Thống kê
```

---

## 🛠️ Hướng dẫn biên dịch và chạy chương trình

### Yêu cầu môi trường

- Windows 10 hoặc Windows 11.
- Trình biên dịch hỗ trợ C++17.
- MSYS2 UCRT64 / MinGW-w64.
- .NET SDK 8.0.
- Microsoft Edge WebView2 Runtime.

### Cách 1: Chạy ứng dụng WebView2 (Khuyên dùng)

**Bước 1: Tải mã nguồn**

```powershell
git clone https://github.com/LuuTrongNguyenn/AnhViet_Dictionary.git
cd AnhViet_Dictionary
```

Nếu file `build_webview2.bat` nằm trong thư mục con `LexiTrie_WebView2_Login`, chuyển vào thư mục đó trước khi chạy.

**Bước 2: Biên dịch và khởi động**

```powershell
.\build_webview2.bat
```

Script thực hiện quá trình build và khởi động các thành phần cần thiết của ứng dụng.

**Bước 3: Sử dụng chương trình**

Sau khi khởi động thành công, ứng dụng sẽ hiển thị giao diện LexiTrie thông qua WebView2 hoặc phương án mở giao diện được script hỗ trợ.

### Cách 2: Chạy phiên bản Console

**Bước 1: Biên dịch mã nguồn C++**

```powershell
g++ -std=c++17 -Iinclude src/main.cpp src/Trie.cpp src/FileManager.cpp src/ConsoleUI.cpp src/SmartReview.cpp -o LexitrieConsole.exe
```

**Bước 2: Chạy chương trình**

```powershell
.\LexitrieConsole.exe
```

Nếu PowerShell không tìm thấy `g++`, có thể thêm đường dẫn MSYS2:

```powershell
$env:PATH = "C:\msys64\ucrt64\bin;" + $env:PATH
```

### Cách 3: Kiểm tra thuật toán Trie

Dự án có file `tests/test_trie.cpp` để phục vụ kiểm tra các thao tác trên cấu trúc dữ liệu Trie.

Ngoài ra, `benchmarks/benchmark.cpp` được sử dụng làm mã nguồn phục vụ đánh giá hiệu năng.

---

## 🌟 Tổng hợp các tính năng chính

Hệ thống LexiTrie tập trung vào các chức năng sau:

1. **Từ điển Anh–Việt:** Tra cứu nghĩa tiếng Việt của từ tiếng Anh.
2. **Trie Search:** Tìm kiếm từ theo cấu trúc Trie.
3. **DFS Autocomplete:** Gợi ý từ theo tiền tố.
4. **Quản lý từ điển:** Thêm, cập nhật và xóa từ.
5. **Dữ liệu UTF-8:** Đọc và lưu từ điển Anh–Việt.
6. **Đăng ký / đăng nhập:** Quản lý tài khoản cục bộ.
7. **Smart Review:** Hỗ trợ học và ôn tập từ vựng.
8. **Lịch sử học tập:** Theo dõi quá trình tra cứu và ôn tập.
9. **Thống kê:** Tổng hợp tiến độ học tập.
10. **WebView2 Dark Mode:** Giao diện Windows hiện đại kết nối backend C++.

---

## 👨‍💻 Phân công công việc nhóm

### Lưu Trọng Nguyên – Trie, DFS và WebView2

**Task Jira:** DSA-3, DSA-5, DSA-7, DSA-16, DSA-18, DSA-19, DSA-25, DSA-27.

**Công việc:**

- Thiết kế TrieNode và lớp Trie.
- Cài đặt thêm, tìm kiếm, cập nhật và xóa từ.
- Xây dựng DFS Autocomplete.
- Hoàn thiện quản lý bộ nhớ.
- Phụ trách giao diện WebView2.
- Viết báo cáo thuật toán, cài đặt chương trình và kết luận.

### Châu Thiện An – Console và Backend

**Task Jira:** DSA-9, DSA-10, DSA-11, DSA-12, DSA-17, DSA-26.

**Công việc:**

- Xây dựng giao diện Console.
- Khởi tạo chương trình C++.
- Cài đặt gợi ý theo từng phím.
- Xây dựng backend API.
- Phối hợp kết nối giao diện với backend.
- Viết báo cáo phân tích yêu cầu và chuẩn bị slide.

### Dương Đình Khánh – Dữ liệu và Smart Review

**Task Jira:** DSA-4, DSA-6, DSA-8, DSA-13, DSA-15, DSA-20.

**Công việc:**

- Chuẩn hóa dữ liệu từ điển Anh–Việt.
- Xây dựng FileManager.
- Đọc, ghi và lưu dữ liệu.
- Phát triển Smart Review.
- Thống kê tiến độ và lịch sử ôn tập.
- Viết báo cáo thực nghiệm.

---

## 📊 Quản lý mã nguồn và tiến độ

### GitHub

**Repository:** https://github.com/LuuTrongNguyenn/AnhViet_Dictionary

- Mỗi thành viên sử dụng tài khoản GitHub riêng.
- Phân chia nhánh theo chức năng.
- Commit mã nguồn với thông điệp rõ ràng.
- Sử dụng Pull Request để hợp nhất thay đổi.
- Theo dõi lịch sử phát triển của dự án.

### Jira

**Dự án:** https://emnguyendz.atlassian.net/jira/software/projects/DSA/list

Nhóm sử dụng Jira để phân công và quản lý tiến độ của **20 task** thuộc các nhóm công việc:

- Trie và thuật toán.
- Dữ liệu và lưu trữ.
- Console và backend API.
- Smart Review.
- Giao diện WebView2.
- Báo cáo và slide thuyết trình.

---

## 📖 Báo cáo đồ án

Báo cáo được biên soạn bằng LaTeX, gồm các nội dung chính:

- **Chương 1:** Tổng quan và phân tích yêu cầu bài toán.
- **Chương 2:** Cấu trúc dữ liệu Trie và thuật toán DFS.
- **Chương 3:** Thiết kế và cài đặt chương trình C++.
- **Chương 4:** Kiểm thử, đánh giá và phân tích hiệu năng.
- **Kết luận:** Kết quả đạt được, hạn chế và hướng phát triển.
- **Phụ lục:** Phân công công việc và tài liệu tham khảo.

---

## 🚀 Hướng phát triển

- Bổ sung phát âm tiếng Anh.
- Phát triển mini-game ghi nhớ từ vựng.
- Tìm kiếm gần đúng khi nhập sai chính tả.
- Cá nhân hóa lịch ôn tập.
- Mở rộng dữ liệu từ điển.
- Tối ưu tốc độ truy vấn và bộ nhớ.

---

**Đề tài ĐT06 – Hệ thống từ điển Anh–Việt và gợi ý từ thông minh (LexiTrie)**

**Nhóm thực hiện:** Châu Thiện An – Lưu Trọng Nguyên – Dương Đình Khánh.
