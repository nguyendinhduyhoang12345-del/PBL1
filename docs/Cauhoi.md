# 📝 Câu Hỏi Vấn Đáp Đồ Án PBL1
### Hệ Thống Quản Lý Quán Cơm Tấm — "Đặt Món Thông Minh, Tối Ưu Thanh Toán"
> **Giáo viên hướng dẫn:** PGS. TS. Trương Ngọc Châu  
> **Sinh viên:** Nguyễn Đình Duy Hoàng & Thái Nguyễn Anh Kiệt

---

## 🏗 PHẦN I — KIẾN TRÚC & THIẾT KẾ HỆ THỐNG

### Câu 1
> **Em hãy giải thích kiến trúc MVC mà nhóm áp dụng trong dự án là gì? Tại sao nhóm lại chọn kiến trúc này thay vì viết tất cả trong một file `main.c` duy nhất?**

*Gợi ý trả lời:* MVC = Model – View – Controller. Model tương ứng với `services/` (logic nghiệp vụ), View tương ứng `ui/` (hiển thị), Controller tương ứng `controllers/` (điều phối luồng). Ưu điểm: tách biệt trách nhiệm, dễ bảo trì, dễ mở rộng, dễ kiểm thử từng phần.

---

### Câu 2
> **Trong project, `models.h` đặt tất cả các struct vào một file duy nhất. Điều này có ưu điểm và nhược điểm gì? Nếu được thiết kế lại, em sẽ cải thiện điều gì?**

*Gợi ý trả lời:* Ưu điểm: dễ tham chiếu chéo, tránh circular dependency. Nhược điểm: khi dự án lớn, file sẽ phình to, khó quản lý. Cải thiện: chia nhỏ theo miền nghiệp vụ (customer_model.h, bill_model.h, ...).

---

### Câu 3
> **Tại sao các biến như `btreeRoot`, `historyHead`, `menuHashTable` lại được khai báo là biến toàn cục trong `main.c`? Điều này có rủi ro gì trong lập trình thực tế?**

*Gợi ý trả lời:* Giúp các module chia sẻ dữ liệu dễ dàng mà không cần truyền con trỏ qua nhiều tầng. Rủi ro: khó kiểm soát ai đang sửa dữ liệu, dễ gây lỗi side effect, khó test unit.

---

## 🌳 PHẦN II — CẤU TRÚC DỮ LIỆU

### Câu 4
> **Nhóm sử dụng B-Tree bậc 4 để quản lý khách hàng. Em hãy giải thích: tại sao B-Tree lại được chọn thay vì một mảng đơn giản hoặc BST thông thường?**

*Gợi ý trả lời:* Mảng → tìm kiếm O(n), BST → có thể mất cân bằng dẫn đến O(n) trong trường hợp xấu. B-Tree luôn cân bằng → đảm bảo O(log n). Phù hợp khi dữ liệu khách hàng ngày càng tăng.

---

### Câu 5
> **Với `BTREE_ORDER = 4`, một node B-Tree có thể chứa tối đa bao nhiêu khóa và bao nhiêu con? Em hãy vẽ minh họa một node đầy và giải thích quy tắc sắp xếp khóa.**

*Gợi ý trả lời:* Tối đa `ORDER - 1 = 3` khóa, tối đa `ORDER = 4` con. Khóa sắp xếp tăng dần. Mọi khóa trong `child[i]` nằm giữa `customers[i-1]` và `customers[i]`.

---

### Câu 6
> **Em hãy mô tả từng bước quá trình chèn một khách hàng mới vào B-Tree khi root đã đầy. Hàm nào được gọi và theo thứ tự nào?**

*Gợi ý trả lời:* `insertBTree()` phát hiện root đầy → tạo root mới `s` → gọi `splitChild(s, 0, root_cũ)` → root cũ bị tách làm 2 node con → `insertNonFull()` đi xuống node con phù hợp để chèn.

---

### Câu 7
> **Tại sao nhóm dùng Doubly Linked List (danh sách liên kết đôi) cho giỏ hàng thay vì Singly Linked List? Việc có con trỏ `prev` giúp ích gì trong chức năng quản lý giỏ hàng?**

*Gợi ý trả lời:* Doubly Linked List cho phép duyệt hai chiều → thuận lợi khi xóa một node ở giữa (không cần duyệt lại từ đầu để tìm node trước). Phù hợp cho thao tác xóa/cập nhật theo index thường xuyên trong giỏ hàng.

---

### Câu 8
> **Hàm `cartRemoveItemByIndex()` hoạt động như thế nào? Em hãy mô tả logic cập nhật con trỏ `prev` và `next` khi xóa một node ở giữa danh sách.**

*Gợi ý trả lời:* Tìm node tại index → `node->prev->next = node->next` → `node->next->prev = node->prev` → `free(node)` → giảm `itemCount`. Xử lý đặc biệt khi xóa node đầu (cập nhật `head`) hoặc node cuối (cập nhật `tail`).

---

### Câu 9
> **Hash Table trong dự án dùng kỹ thuật xử lý đụng độ nào? Hàm băm được tính ra sao? Khi có đụng độ thì điều gì xảy ra?**

*Gợi ý trả lời:* Dùng **chaining** (danh sách liên kết đơn tại mỗi bucket). Hàm băm: `key % HASH_TABLE_SIZE` (với `HASH_TABLE_SIZE = 101`). Khi đụng độ, node mới được thêm vào đầu (hoặc cuối) của linked list tại bucket đó.

---

### Câu 10
> **Tại sao `HASH_TABLE_SIZE = 101` được chọn là số nguyên tố? Điều này ảnh hưởng như thế nào đến hiệu năng của hash table?**

*Gợi ý trả lời:* Số nguyên tố giúp phân phối đều hơn khi dùng phép chia lấy dư (modulo), giảm đụng độ. Nếu dùng số chẵn hoặc có nhiều ước thì các key có cùng ước chung sẽ ánh xạ về cùng bucket.

---

## ⚙️ PHẦN III — LOGIC NGHIỆP VỤ

### Câu 11
> **Em hãy mô tả toàn bộ luồng xử lý từ lúc khách chọn món đến khi hóa đơn được in ra. Liệt kê các hàm và module được gọi theo thứ tự.**

*Gợi ý trả lời:* `main.c` → `handleOrderMenu()` → `menu_ui` hiển thị → `getValidInt()` validate → `cartAddItem()` thêm vào giỏ → `handleCustomer()` tra cứu/tạo KH → `processCheckout()` → `checkout_service` tính tiền → `history_service` lưu hóa đơn → `bill_ui` in hóa đơn.

---

### Câu 12
> **Hệ thống hạng thành viên hoạt động như thế nào? Mức giảm giá được tính và áp dụng vào hóa đơn ở đâu trong code?**

*Gợi ý trả lời:* 4 hạng: Bronze (0%), Silver ≥ 500k (10%), Gold ≥ 2tr (20%), Diamond ≥ 50tr (30%). Logic trong `checkout_service.c`: sau khi tính `totalPrice`, nhân với `(1 - discountRate)`. Sau thanh toán, cộng `finalPrice` vào `customer.totalSpent` rồi cập nhật rank.

---

### Câu 13
> **Khi khách hàng nhập số điện thoại để tra cứu, luồng xử lý trong `customer_controller` diễn ra như thế nào? Nếu khách chưa tồn tại thì hệ thống làm gì?**

*Gợi ý trả lời:* Gọi `searchBTree(btreeRoot, phone)` → nếu tìm thấy: load thông tin vào `currentBill.customer` → nếu không: tạo `Customer` mới → gọi `insertBTree()` → ghi vào `khachhang.txt` qua `file_service`.

---

### Câu 14
> **Dữ liệu lịch sử hóa đơn được lưu vào những file nào? Tại sao lại tách thành cả file chung (`history.txt`) và file riêng theo từng khách hàng (`history_khach_N.txt`)?**

*Gợi ý trả lời:* File chung để thống kê toàn bộ doanh thu. File riêng để tra cứu nhanh lịch sử của một khách cụ thể mà không cần duyệt toàn bộ danh sách. Đây là cách tối ưu read performance theo use-case.

---

### Câu 15
> **Chức năng "Thống kê doanh thu" (CN6) đọc dữ liệu từ file hay từ danh sách trong bộ nhớ? Tại sao lại thiết kế như vậy?**

*Gợi ý trả lời:* Đọc từ `history.txt` qua `report_service`. Lý do: đảm bảo dữ liệu thống kê chính xác ngay cả khi chương trình được khởi động lại nhiều lần (dữ liệu persistent), không bị mất khi program crash.

---

## 🖥 PHẦN IV — QUẢN LÝ BỘ NHỚ & LẬP TRÌNH C

### Câu 16
> **Trong `btree_service.c`, hàm `createBTreeNode()` dùng `malloc` để cấp phát bộ nhớ. Khi nào bộ nhớ này được giải phóng? Nếu không giải phóng, điều gì xảy ra?**

*Gợi ý trả lời:* Theo tài liệu, dự án hiện chưa có hàm `freeBTree()`. Khi không giải phóng → memory leak: bộ nhớ bị chiếm mãi cho đến khi OS thu hồi lúc chương trình kết thúc. Trong chương trình chạy ngắn thì ít ảnh hưởng, nhưng với server chạy liên tục thì rất nguy hiểm.

---

### Câu 17
> **Tại sao B-Tree lưu `Customer*` (con trỏ) thay vì lưu trực tiếp `Customer` (struct)? Điều này ảnh hưởng đến bộ nhớ như thế nào?**

*Gợi ý trả lời:* Lưu con trỏ giúp tránh copy struct (tiết kiệm bộ nhớ khi struct lớn), các thay đổi trên `Customer` sẽ được phản ánh ngay qua con trỏ mà không cần cập nhật lại tree. Nhược điểm: phải quản lý vòng đời của vùng nhớ được trỏ đến.

---

### Câu 18
> **`validator.c` có các hàm kiểm tra đầu vào. Em hãy giải thích tại sao việc validate đầu vào lại quan trọng trong lập trình C, và nêu một ví dụ lỗi có thể xảy ra nếu không validate số điện thoại.**

*Gợi ý trả lời:* C không tự động xử lý lỗi nhập sai kiểu. Nếu không validate: `scanf("%d", ...)` với input là chữ → undefined behavior, buffer không được xóa → vòng lặp vô hạn. Với SĐT: nếu nhập chữ → `strcmp` trong B-Tree sẽ cho kết quả sai.

---

### Câu 19
> **`file_service.c` dùng `fopen`, `fprintf`, `fscanf` để đọc/ghi file. Nếu file `menu.txt` bị xóa hoặc bị lỗi khi mở, chương trình sẽ xử lý thế nào? Nhóm có kiểm tra điều này không?**

*Gợi ý trả lời:* Câu hỏi mở — sinh viên cần kiểm tra code thực tế. Nếu không check `FILE* f == NULL` thì truy cập vào con trỏ NULL → segmentation fault. Nên thêm kiểm tra và thông báo lỗi phù hợp.

---

## 🔄 PHẦN V — LUỒNG DỮ LIỆU & TÍCH HỢP MODULE

### Câu 20
> **Khi chương trình khởi động, 3 hàm `loadMenuFromFile()`, `loadCustomersFromFile()`, `loadHistoryFromFile()` được gọi. Em hãy giải thích từng hàm load dữ liệu vào cấu trúc dữ liệu nào trong bộ nhớ.**

*Gợi ý trả lời:* `loadMenuFromFile()` → mảng `menu[]` + `menuHashTable`. `loadCustomersFromFile()` → mảng `danhSachKH[]` + `btreeRoot` (B-Tree). `loadHistoryFromFile()` → `historyHead` (Singly Linked List).

---

### Câu 21
> **Sau khi thanh toán xong, hệ thống cần cập nhật những dữ liệu gì và ở đâu (trong bộ nhớ và trên file)? Liệt kê đầy đủ.**

*Gợi ý trả lời:*  
- Trong bộ nhớ: `currentBill.customer.totalSpent` tăng, rank được cập nhật, node HistoryNode mới được thêm vào `historyHead`.  
- Trên file: ghi hóa đơn vào `history.txt`, ghi vào `history_khach_N.txt`, cập nhật `khachhang.txt` với tổng chi tiêu mới.  
- Reset `currentBill` về trạng thái trống để đón khách tiếp.

---

### Câu 22
> **Nếu nhóm muốn thêm tính năng "Coupon giảm giá" (nhập mã coupon khi thanh toán), em sẽ cần thêm/sửa những module nào? Hãy đề xuất hướng mở rộng.**

*Gợi ý trả lời:* Câu hỏi mở — đánh giá tư duy thiết kế. Cần thêm struct `Coupon`, một `coupon_service.c`, sửa `checkout_service.c` để apply coupon trước khi tính `finalPrice`, thêm UI để nhập mã, thêm file `coupons.txt` để lưu danh sách coupon hợp lệ.

---

## 💡 PHẦN VI — CÂU HỎI TỔ HỢPL & TƯ DUY PHẢN BIỆN

### Câu 23
> **Nhóm chọn lưu trữ dữ liệu bằng file text (`.txt`). So với dùng cơ sở dữ liệu (SQLite, MySQL), cách này có ưu nhược điểm gì? Với quy mô một quán cơm thực tế, cách nào phù hợp hơn?**

*Gợi ý trả lời:* File text: đơn giản, không cần phụ thuộc thêm, phù hợp môn học. Nhược: không có transaction, dễ mất dữ liệu nếu crash khi đang ghi, không hỗ trợ query phức tạp. DB: mạnh mẽ hơn nhưng cần kiến thức SQL và thư viện bổ sung.

---

### Câu 24
> **Độ phức tạp thời gian (time complexity) của các thao tác chính trong dự án là bao nhiêu? Em hãy phân tích cho: Tìm khách hàng theo SĐT, Thêm món vào giỏ hàng, Tra cứu món ăn theo ID.**

*Gợi ý trả lời:*  
- Tìm khách hàng (B-Tree search): **O(log n)**  
- Thêm vào giỏ (append to Doubly LL): **O(1)** (nếu có con trỏ `tail`)  
- Tra cứu món ăn (Hash Table): **O(1)** trung bình, **O(k)** xấu nhất với k là số phần tử cùng bucket

---

### Câu 25
> **Nếu quán cơm có 10,000 khách hàng, B-Tree bậc 4 sẽ có chiều cao tối đa là bao nhiêu tầng? So sánh với mảng thông thường khi tìm kiếm.**

*Gợi ý trả lời:* Chiều cao B-Tree bậc 4: `h ≤ log₂(10000) ≈ 13` tầng (thực tế ít hơn vì mỗi node chứa nhiều khóa). Tìm kiếm B-Tree: tối đa ~13 lần so sánh. Mảng tuần tự: tối đa 10,000 lần so sánh. B-Tree nhanh hơn ~770 lần.

---

### Câu 26 — Câu hỏi tình huống
> **Tình huống: Khách đang đặt món (giỏ hàng có 3 món), sau đó nhập thông tin khách hàng và phát hiện tồn kho của một món đã hết. Hệ thống hiện tại xử lý tình huống này như thế nào? Có vấn đề gì không?**

*Gợi ý trả lời:* Câu hỏi mở để đánh giá hiểu biết sâu. Tồn kho giảm khi nào? Khi chọn món hay khi thanh toán? Nếu giảm khi chọn món → OK nhưng cần rollback nếu hủy. Nếu giảm khi thanh toán → race condition nếu có nhiều đơn. Sinh viên cần trình bày giải pháp nhóm đã chọn.

---

*📌 Tài liệu câu hỏi này được tổng hợp dựa trên toàn bộ codebase và tài liệu của dự án PBL1.*  
*🗓 Ngày tạo: 23/06/2026*
