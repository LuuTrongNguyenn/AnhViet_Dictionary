<div align="center">

# 🐱 MOCHIQUEST — KINGDOM OF WORDS

### 📚 LEXITRIE · HỆ THỐNG TỪ ĐIỂN ANH–VIỆT THÔNG MINH

**Ứng dụng tra cứu từ điển bằng Trie kết hợp học tiếng Anh và phiêu lưu tương tác**

![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Trie](https://img.shields.io/badge/Data%20Structure-Trie-8B5CF6?style=for-the-badge)
![WebView2](https://img.shields.io/badge/Desktop-WebView2-2563EB?style=for-the-badge)
![JavaScript](https://img.shields.io/badge/Frontend-JavaScript-F7DF1E?style=for-the-badge&logo=javascript&logoColor=black)
![Windows](https://img.shields.io/badge/Platform-Windows-0078D4?style=for-the-badge&logo=windows&logoColor=white)

**Đồ án môn Cấu trúc dữ liệu và Giải thuật (CTDL & GT) — Đề tài ĐT06**

[📂 Mã nguồn GitHub](https://github.com/LuuTrongNguyenn/AnhViet_Dictionary) · [🚀 Hướng dẫn chạy](#-hướng-dẫn-cài-đặt--khởi-chạy) · [🧪 Kiểm thử](#-kiểm-thử--đánh-giá-hiệu-năng)

</div>

---

## 📑 Mục lục

- [🌟 Giới thiệu tổng quan](#-giới-thiệu-tổng-quan)
- [🏗️ Kiến trúc hệ thống](#️-kiến-trúc-hệ-thống)
- [📁 Cấu trúc thư mục dự án](#-cấu-trúc-thư-mục-dự-án)
- [✨ Các tính năng chính](#-các-tính-năng-chính)
- [🧩 Cấu trúc dữ liệu và thuật toán](#-cấu-trúc-dữ-liệu-và-thuật-toán)
- [🔄 Các phiên bản ứng dụng](#-các-phiên-bản-ứng-dụng)
- [🚀 Hướng dẫn cài đặt và khởi chạy](#-hướng-dẫn-cài-đặt-và-khởi-chạy)
- [🧪 Kiểm thử và đánh giá hiệu năng](#-kiểm-thử-và-đánh-giá-hiệu-năng)
- [🔒 Dữ liệu và bảo mật](#-dữ-liệu-và-bảo-mật)
- [👥 Thành viên và phân công](#-thành-viên-và-phân-công)

---

## 🌟 Giới thiệu tổng quan

**MochiQuest — Kingdom of Words** là giao diện học tiếng Anh theo phong cách phiêu lưu của dự án **LexiTrie**, một hệ thống từ điển Anh–Việt phát triển bằng **C++17**. Trọng tâm học thuật của đồ án là **cấu trúc dữ liệu Trie (Prefix Tree)** để quản lý từ vựng và **DFS (Depth-First Search)** để gợi ý từ theo tiền tố.

Ứng dụng kết hợp công cụ tra cứu với hoạt động học tập, giúp người dùng vừa tìm nghĩa của từ, vừa ôn luyện qua các thử thách tương tác.

### Điểm nổi bật

- 📖 **Kho từ điển khoảng 77.568 mục từ** trong `data/dictionary.txt`.
- ⚡ **Tra cứu, thêm và xóa từ** bằng cấu trúc dữ liệu Trie.
- 🔎 **Autocomplete:** Gợi ý từ tiếng Anh khi nhập tiền tố.
- 🧠 **Smart Review:** Theo dõi và ôn tập các từ đã học.
- 🐱 **MochiQuest:** Giao diện thế giới phép thuật với khu học, tim, XP và thử thách 100 ải.
- 🔐 **Tài khoản cục bộ:** Đăng ký, đăng nhập và quản lý phiên trong bản có backend.
- 🧪 **Kiểm thử và benchmark:** Kiểm tra thao tác Trie và đánh giá thời gian xử lý.


---

## 🏗️ Kiến trúc hệ thống

Hệ thống tổ chức thành các thành phần tách biệt:

| Thành phần | Công nghệ | Vai trò |
|---|---|---|
| **Core Engine** | C++17 | Quản lý dữ liệu từ điển và cấu trúc Trie |
| **Thuật toán** | Trie + DFS | Tìm từ và gợi ý theo tiền tố |
| **Backend** | C++ / Winsock | Cung cấp HTTP API cho giao diện |
| **Frontend** | HTML, CSS, JavaScript | Hiển thị giao diện từ điển và MochiQuest |
| **Desktop Host** | .NET 8 / Microsoft WebView2 | Chạy giao diện trong cửa sổ ứng dụng Windows |
| **Lưu trữ** | Tệp UTF-8 và dữ liệu cục bộ | Từ điển, thông tin học tập và tài khoản |

**Luồng xử lý chính:**

```text
Người dùng
    │
    ▼
Giao diện MochiQuest / LexiTrie (HTML, CSS, JavaScript)
    │  HTTP API
    ▼
Backend C++ (WebServer.cpp)
    │
    ├── Trie + DFS ────────── Tra cứu và gợi ý từ
    ├── FileManager ──────── Đọc / ghi dữ liệu từ điển
    └── SmartReview ──────── Theo dõi và ôn tập từ vựng
```


---

## 📁 Cấu trúc thư mục dự án

```text
AnhViet_Dictionary/
├── README.md                    # Giới thiệu, kiến trúc và hướng dẫn sử dụng dự án
├── build_webview2.bat           # Biên dịch backend C++ và khởi chạy ứng dụng Windows
├── .gitattributes               # Thiết lập quy tắc xử lý tệp trong Git
│
├── include/                     # Khai báo lớp, cấu trúc và giao diện hàm C++
│   ├── Trie.h                   # Định nghĩa cây Trie và các thao tác từ vựng
│   ├── FileManager.h            # Khai báo chức năng nạp/lưu dữ liệu từ điển
│   ├── SmartReview.h            # Khai báo mô hình và chức năng ôn tập
│   └── ConsoleUI.h              # Khai báo giao diện dòng lệnh
│
├── src/                         # Triển khai thuật toán và nghiệp vụ C++
│   ├── Trie.cpp                 # Cài đặt thêm, tìm, xóa từ và gợi ý theo tiền tố
│   ├── FileManager.cpp          # Đọc/ghi và xử lý dữ liệu từ điển
│   ├── SmartReview.cpp          # Xử lý kết quả học và thống kê ôn tập
│   ├── ConsoleUI.cpp            # Hiển thị menu và xử lý tương tác console
│   ├── WebServer.cpp            # HTTP API nội bộ kết nối frontend với C++
│   └── main.cpp                 # Điểm khởi chạy chương trình console
│
├── data/                        # Dữ liệu phục vụ tra cứu và phân loại từ
│   ├── dictionary.txt           # Kho từ điển Anh–Việt (khoảng 77.568 mục từ)
│   ├── word_parts.txt           # Dữ liệu phân loại từ vựng theo từ loại
│   └── word_topics.txt          # Dữ liệu nhóm từ vựng theo chủ đề
│
├── web/                         # Giao diện người dùng HTML/CSS/JavaScript
│   ├── index.html               # Trang chính LexiTrie / MochiQuest
│   ├── style.css                # Định dạng bố cục, màu sắc và thành phần UI
│   ├── app.js                   # Tương tác tra cứu và xử lý giao diện
│   ├── login.css                # Kiểu dáng màn hình đăng nhập/đăng ký
│   ├── login.js                 # Xử lý tương tác xác thực trên giao diện
│   ├── mochi-academy-v10.js     # Logic các khu học và thử thách MochiQuest
│   ├── mochi-integration.js     # Kết nối tính năng MochiQuest với giao diện
│   └── mochi-vocabulary.json    # Bộ dữ liệu từ vựng cho hoạt động học tập
│
├── WebViewHost/                 # Ứng dụng host desktop trên Windows
│   ├── Program.cs               # Khởi tạo cửa sổ và điều khiển WebView2
│   └── WebViewHost.csproj        # Cấu hình dự án .NET và phụ thuộc WebView2
│
├── tests/                       # Mã kiểm thử thuật toán
│   └── test_trie.cpp            # Kiểm tra các thao tác của cây Trie
│
└── benchmarks/                  # Đánh giá hiệu năng cấu trúc dữ liệu
    └── benchmark.cpp           # Đo thời gian thực thi các thao tác Trie
```

---

## ✨ Các tính năng chính

### 1. 📖 Tra cứu từ điển Anh–Việt

- Tìm từ tiếng Anh và hiển thị nghĩa tiếng Việt.
- Thông báo khi từ không tồn tại.
- Kết nối giao diện với backend C++ để tra cứu dữ liệu từ điển.
- Hỗ trợ thêm, cập nhật hoặc xóa từ theo chức năng backend.

### 2. 🔎 Autocomplete — Gợi ý từ theo tiền tố

- Nhận tiền tố người dùng đang nhập.
- Truy cập nhánh tương ứng trên Trie và duyệt DFS để thu thập các từ phù hợp.
- Giới hạn số gợi ý hiển thị nhằm giữ giao diện dễ sử dụng.

### 3. 🧠 Smart Review — Ôn tập thông minh

- Lưu từ vựng cần học và theo dõi quá trình ôn tập.
- Ghi nhận kết quả đúng / sai, thống kê và lịch sử.
- Cho phép luyện lại từ đã học theo mục tiêu củng cố trí nhớ.

### 4. 🐱 MochiQuest — Kingdom of Words

Giao diện học tập được thiết kế theo phong cách **Dark Fantasy**, kết hợp bản đồ phiêu lưu, khu học và hệ thống phần thưởng.

| Khu vực | Nội dung học tập |
|---|---|
| 📚 **Thư viện Từ vựng** | Học từ mới, nghĩa và ví dụ |
| 🧠 **Đền Trí Nhớ** | Ôn lại từ đã học với Smart Review |
| 🎧 **Phòng Luyện nghe** | Nghe và nhận diện từ hoặc câu tiếng Anh |
| ✍️ **Tháp Ngữ pháp** | Luyện cấu trúc câu và ngữ pháp |
| 📖 **Vườn Đọc hiểu** | Đọc đoạn văn và trả lời câu hỏi |
| ⚔️ **Thử thách 100 ải** | Học qua các màn chơi, XP và hệ thống tim |


### 5. 🔐 Tài khoản cục bộ

- Đăng ký, đăng nhập và đăng xuất qua backend.
- Sử dụng phiên đăng nhập trong ứng dụng cục bộ.
- Không đưa dữ liệu tài khoản thật lên repository công khai.

### 6. 📊 Kiểm thử và benchmark

- Kiểm tra các thao tác chính của Trie.
- Đo thời gian tìm kiếm / thao tác dữ liệu bằng chương trình benchmark.
- Kết quả phụ thuộc môi trường và dữ liệu thực tế.

---

## 🧩 Cấu trúc dữ liệu và thuật toán

**Trie (Prefix Tree)** là cấu trúc dữ liệu trung tâm của đồ án. Các từ chia sẻ tiền tố sẽ dùng chung một phần đường đi trong cây.

| Thao tác | Nguyên lý | Độ phức tạp điển hình |
|---|---|---|
| **Thêm từ** | Duyệt hoặc tạo nút theo từng ký tự | `O(L)` |
| **Tìm từ** | Duyệt đường đi ứng với chuỗi cần tìm | `O(L)` |
| **Xóa từ** | Bỏ dấu kết thúc từ và dọn nút không cần thiết | `O(L)` |
| **Gợi ý từ** | Tìm nút tiền tố, sau đó duyệt DFS | Phụ thuộc số nút duyệt và số kết quả |

`L` là độ dài từ, giả định thao tác truy cập nhánh ký tự có chi phí hằng số.

### Vì sao chọn Trie?

1. Tận dụng tiền tố chung giữa các từ tiếng Anh.
2. Tìm kiếm dựa trên độ dài chuỗi thay vì duyệt tuyến tính toàn bộ danh sách.
3. Hỗ trợ gợi ý từ theo tiền tố một cách tự nhiên.
4. Phù hợp với mục tiêu học thuật của đề tài ĐT06.

---

## 🔄 Các phiên bản ứng dụng

| Phiên bản | Mục đích | Cách chạy |
|---|---|---|
| **C++ Console** | Thực hành và kiểm thử thuật toán | Biên dịch các file C++ phù hợp |
| **C++ + WebView2** | Bản ứng dụng Windows chính | `build_webview2.bat` |
| **MochiQuest Web độc lập** | Bản trình diễn HTML trên trình duyệt | Máy chủ tĩnh / GitHub Pages |

GitHub Pages phục vụ giao diện HTML/CSS/JavaScript tĩnh; backend C++ của bản WebView2 chạy riêng trên Windows.

---

## 🚀 Hướng dẫn cài đặt và khởi chạy

### 1. Yêu cầu môi trường

- **Windows 10/11**.
- **C++17** với MSYS2 UCRT64 / MinGW-w64; cấu hình hiện tại sử dụng `C:\msys64\ucrt64\bin\g++.exe`.
- **.NET 8 SDK** để build WebView2 host.
- **Microsoft Edge WebView2 Runtime** để chạy ứng dụng desktop.


### 2. Tải mã nguồn

```powershell
git clone https://github.com/LuuTrongNguyenn/AnhViet_Dictionary.git
cd AnhViet_Dictionary
```

Nếu tải ZIP, hãy giải nén và mở PowerShell **tại thư mục thực sự chứa `build_webview2.bat`**.

### 3. Khởi chạy nhanh trên Windows

```powershell
$env:PATH = "C:\msys64\ucrt64\bin;C:\msys64\usr\bin;" + $env:PATH
.\build_webview2.bat
```

Script dự kiến thực hiện:

1. Biên dịch backend C++ thành `LexiTrieBackend.exe`.
2. Build ứng dụng host WebView2.
3. Khởi động backend và mở cửa sổ ứng dụng.

Nếu host không mở, có thể kiểm tra địa chỉ cục bộ:

**http://127.0.0.1:18765**

Địa chỉ này chỉ hoạt động trên máy đang chạy backend, **không phải link website công khai**.

### 4. Nếu PowerShell không tìm thấy file `.bat`

```powershell
$file = Get-ChildItem -Path . -Recurse -Filter "build_webview2.bat" -File | Select-Object -First 1
if ($file) {
    Set-Location $file.DirectoryName
    & $file.FullName
} else {
    Write-Host "Khong tim thay build_webview2.bat"
}
```

### 5. Dừng backend

```powershell
taskkill /IM LexiTrieBackend.exe /F
```

### 6. Chạy bản HTML độc lập (nếu có repository web riêng)

Dùng một static server để phục vụ `index.html` và các file JavaScript / JSON liên quan. Có thể triển khai lên GitHub Pages để chia sẻ giao diện và các chức năng chạy thuần trình duyệt. **Bản này không tự động đồng bộ tài khoản hoặc sử dụng backend Trie C++.**

---

## 🧪 Kiểm thử và đánh giá hiệu năng

### 1. Kiểm thử Trie

```powershell
& "C:\msys64\ucrt64\bin\g++.exe" -std=c++17 -Iinclude tests\test_trie.cpp src\Trie.cpp src\FileManager.cpp -o test_trie.exe
.\test_trie.exe
```

### 2. Benchmark Trie

```powershell
& "C:\msys64\ucrt64\bin\g++.exe" -std=c++17 -O2 -Iinclude benchmarks\benchmark.cpp src\Trie.cpp -o benchmark.exe
.\benchmark.exe
```

### 3. Kiểm thử tích hợp thủ công

- [ ] Backend C++ biên dịch và khởi động thành công.
- [ ] Giao diện WebView2 / trình duyệt tải được.
- [ ] Tra cứu từ `apple` và nhận nghĩa tiếng Việt.
- [ ] Autocomplete trả về các từ có cùng tiền tố.
- [ ] Đăng ký / đăng nhập / đăng xuất hoạt động.
- [ ] Smart Review lưu và đọc lại tiến độ.
- [ ] Các khu học MochiQuest hiển thị đúng dạng bài.
- [ ] Thử thách 100 ải, tim và XP hoạt động theo thiết kế.


---

## 🔒 Dữ liệu và bảo mật

- Từ điển được lưu trong `data/dictionary.txt` và các tệp dữ liệu hỗ trợ.
- File `data/accounts.db` chứa thông tin xác thực cục bộ; **không commit lên GitHub**.
- Không commit cookie, session, khóa bí mật hoặc dữ liệu tài khoản người dùng.
- Loại bỏ sản phẩm build như `*.exe`, `bin/`, `obj/`, `publish/` khỏi commit khi không cần thiết.
- Backend mặc định lắng nghe tại `127.0.0.1:18765` và phục vụ ứng dụng trên máy cục bộ.
- Không mở trực tiếp backend lên Internet khi chưa kiểm tra xác thực, phân quyền và bảo vệ API.

Gợi ý `.gitignore`:

```gitignore
*.exe
*.dll
*.pdb
bin/
obj/
publish/
.vs/
data/accounts.db
```

---

## 👥 Thành viên và phân công

| STT | Họ và tên | Mã sinh viên | Phần phụ trách | Thành phần |
|:---:|---|:---:|---|---|
| 1 | **Châu Thiện An** | `6651071002` | Console và backend API | `ConsoleUI.*`, `src/main.cpp`, `src/WebServer.cpp` |
| 2 | **Lưu Trọng Nguyên** | `6651071048` | Trie, giao diện WebView2, benchmark và kiểm thử Trie | `Trie.*`, `web/`, `WebViewHost/`, `benchmarks/`, `tests/` |
| 3 | **Dương Đình Khánh** | `6651071033` | Quản lý dữ liệu và Smart Review | `FileManager.*`, `SmartReview.*`, `data/` |

Mỗi thành viên phát triển trên nhánh chức năng riêng, commit bằng tài khoản cá nhân và tạo Pull Request khi tích hợp vào nhánh chính.

---

<div align="center">

### 🐱 MOCHIQUEST · KINGDOM OF WORDS

**LEXITRIE · C++17 · TRIE + DFS · CTDL & GT · ĐT06**

*Khám phá ngôn từ — Chinh phục thế giới.*

</div>
