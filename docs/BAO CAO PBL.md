# MỞ ĐẦU

Báo cáo Đồ án Lập trình Tính toán "Đặt món thông minh, tối ưu thanh toán" là kết quả của quá trình học tập và tìm hiểu của nhóm trong học phần PBL1. Đề tài hướng đến xây dựng một hệ thống quản lý quán cơm tấm bằng ngôn ngữ C, hỗ trợ các chức năng cơ bản như đặt món, quản lý giỏ hàng, nhập thông tin khách hàng, thanh toán, xem lịch sử và thống kê doanh thu.

Trong quá trình thực hiện, nhóm đã cố gắng vận dụng các kiến thức đã học về cấu trúc dữ liệu, lập trình module và xử lý file để hoàn thiện chương trình. Mặc dù đã nỗ lực trong quá trình làm việc, báo cáo vẫn khó tránh khỏi những thiếu sót. Nhóm rất mong nhận được sự góp ý của thầy cô để hoàn thiện hơn trong những phần sau.

Nhóm chúng em xin chân thành cảm ơn!

# I. TỔNG QUAN ĐỀ TÀI

Trong thời đại công nghệ thông tin phát triển mạnh mẽ, việc ứng dụng phần mềm vào quản lý bán hàng giúp nâng cao hiệu quả làm việc, giảm sai sót và tiết kiệm thời gian xử lý. Từ thực tế đó, nhóm em lựa chọn thực hiện đề tài “Đặt món thông minh, tối ưu thanh toán” nhằm xây dựng một hệ thống quản lý quán cơm tấm đơn giản nhưng có tính ứng dụng cao.

Đề tài tập trung vào các chức năng chính như đặt món, quản lý giỏ hàng, nhập thông tin khách hàng, thanh toán hóa đơn, xem lịch sử đơn hàng và thống kê doanh thu. Trong quá trình thực hiện, nhóm đã vận dụng các kiến thức đã học về lập trình C, cấu trúc dữ liệu, xử lý file và tổ chức chương trình theo mô hình module để hoàn thiện hệ thống.

1. Mục đích thực hiện đề tài
- Xây dựng một chương trình hỗ trợ quản lý quán cơm tấm bằng ngôn ngữ C.
- Rèn luyện khả năng phân tích bài toán, thiết kế chương trình và xử lý dữ liệu.

2. Mục tiêu của đề tài
- Tạo ra một hệ thống có thể phục vụ việc đặt món và thanh toán nhanh chóng.
- Hỗ trợ lưu trữ, tra cứu thông tin khách hàng và lịch sử đơn hàng thuận tiện.

3. Phạm vi và đối tượng nghiên cứu
- Tìm hiểu và áp dụng các cấu trúc dữ liệu phù hợp để quản lý menu, giỏ hàng và khách hàng.
- Xây dựng chương trình trong phạm vi mô phỏng hoạt động của một quán cơm tấm.

4. Phương pháp nghiên cứu
- Nghiên cứu tài liệu và tham khảo các kiến thức đã học.
- Phân tích yêu cầu, thiết kế chương trình và triển khai bằng ngôn ngữ C.

5. Cấu trúc đồ án môn học
- Phần 1: Tổng quan đề tài.
- Phần 2: Cơ sở lý thuyết.
- Phần 3: Tổ chức cấu trúc dữ liệu và thuật toán.
- Phần 4: Chương trình và kết quả.
- Phần 5: Kết luận và hướng phát triển.

# II. CƠ SỞ LÝ THUYẾT

## 1. Ý tưởng

Đề tài được xây dựng từ nhu cầu thực tế trong việc quản lý đặt món và thanh toán tại quán cơm tấm. Thay vì xử lý thủ công, hệ thống được thiết kế để hỗ trợ nhân viên thao tác nhanh hơn, đồng thời giúp quản lý dữ liệu món ăn, khách hàng và hóa đơn một cách rõ ràng, thuận tiện.

- Ứng dụng cho phép đặt món, quản lý giỏ hàng và thanh toán trên cùng một hệ thống.
- Dữ liệu được lưu trữ và xử lý bằng các cấu trúc phù hợp để tăng tốc độ tìm kiếm và cập nhật.
- Hệ thống còn hỗ trợ xem lịch sử đơn hàng và thống kê doanh thu để phục vụ việc quản lý.

## 2. Cấu trúc dữ liệu và công nghệ sử dụng

### 2.1. Kiểu cấu trúc `MenuItem`

`MenuItem` được dùng để lưu thông tin một món ăn trong menu. Đây là kiểu dữ liệu trung tâm để hiển thị danh sách món, tra cứu món và thêm món vào giỏ hàng.

- `id`: mã món ăn.
- `name`: tên món.
- `price`: đơn giá của món.
- `hasOptions`: đánh dấu món có lựa chọn phụ hay không.

### 2.2. Kiểu cấu trúc `OrderItem`

`OrderItem` biểu diễn một món đã được đặt trong giỏ hàng hoặc hóa đơn. Cấu trúc này lưu đầy đủ thông tin cần thiết để tính tiền và in chi tiết đơn hàng.

- `id`: mã món đã chọn.
- `name`: tên món đã đặt.
- `price`: đơn giá của món.
- `quantity`: số lượng đặt.
- `option`: lựa chọn phụ của món nếu có.
- `note`: ghi chú thêm cho món.
- `totalPrice`: thành tiền của món trong giỏ.

### 2.3. Kiểu cấu trúc `Customer`

`Customer` dùng để quản lý thông tin khách hàng và phục vụ việc tích lũy doanh thu theo từng khách.

- `id`: mã khách hàng.
- `phone`: số điện thoại.
- `name`: họ tên khách hàng.
- `address`: địa chỉ khách hàng.
- `totalSpent`: tổng tiền đã chi tiêu.
- `rank`: hạng thành viên hiện tại.

### 2.4. Danh sách liên kết đôi `CartNode` và `Cart`

Giỏ hàng được cài đặt bằng danh sách liên kết đôi để thuận tiện cho việc thêm món, xóa món và cập nhật số lượng.

a) `CartNode`

- `item`: lưu một món trong giỏ hàng dưới dạng `OrderItem`.
- `prev`: con trỏ trỏ tới phần tử đứng trước trong giỏ hàng.
- `next`: con trỏ trỏ tới phần tử đứng sau trong giỏ hàng.

`CartNode` là thành phần cơ bản để tạo nên danh sách giỏ hàng liên kết đôi.

b) `Cart`

- `head`: con trỏ tới phần tử đầu tiên của giỏ hàng.
- `tail`: con trỏ tới phần tử cuối cùng của giỏ hàng.
- `itemCount`: số lượng món hiện có trong giỏ hàng.

`Cart` dùng để quản lý toàn bộ danh sách món đã chọn, hỗ trợ duyệt giỏ hàng theo hai chiều và xử lý từng món một cách linh hoạt.

### 2.5. Kiểu cấu trúc `Bill`

`Bill` lưu toàn bộ dữ liệu của một lần thanh toán, bao gồm khách hàng, giỏ hàng và các giá trị tiền tệ liên quan.

- `id`: mã hóa đơn.
- `customer`: thông tin khách hàng.
- `cart`: giỏ hàng tại thời điểm thanh toán.
- `total`: tổng tiền trước giảm giá.
- `discount`: số tiền được giảm.
- `finalPrice`: số tiền phải thanh toán.
- `dateTime`: thời gian lập hóa đơn.

### 2.6. Kiểu cấu trúc `CustomerRevenue`

`CustomerRevenue` được dùng để thống kê doanh thu theo khách hàng, hỗ trợ cho phần báo cáo và phân tích dữ liệu bán hàng.

- `customerId`: mã khách hàng.
- `billCount`: số lượng hóa đơn của khách hàng.
- `revenue`: tổng doanh thu mà khách đã tạo ra.

### 2.7. Danh sách liên kết đơn `HistoryNode`

`HistoryNode` dùng để lưu lịch sử các hóa đơn đã thanh toán. Mỗi nút chứa một hóa đơn và con trỏ sang nút tiếp theo.

- `customerId`: mã khách hàng.
- `bill`: dữ liệu của hóa đơn.
- `next`: con trỏ sang hóa đơn tiếp theo.

### 2.8. Cây B-Tree `BTreeNode`

`BTreeNode` là cấu trúc cây tìm kiếm cân bằng, được dùng để tra cứu khách hàng theo khóa một cách nhanh và ổn định khi dữ liệu lớn.

- `customers`: mảng con trỏ chứa các khách hàng.
- `children`: mảng con trỏ đến các nút con.
- `numKeys`: số khóa hiện có trong nút.
- `isLeaf`: đánh dấu nút lá hay không.

Trong hệ thống, B-Tree giúp tìm khách hàng theo số điện thoại nhanh hơn so với duyệt tuần tự.

### 2.9. Bảng băm `HashNode` và `HashTable`

Bảng băm được sử dụng để tra cứu menu theo mã món, giúp truy xuất dữ liệu nhanh hơn trong quá trình đặt món.

- `HashNode` chứa khóa, một `MenuItem` và con trỏ `next`.
- `HashTable` gồm mảng bảng băm `table` và kích thước `size`.

Cách cài đặt này phù hợp với chức năng tìm kiếm món nhanh trong chương trình.

### 2.10. Tổ chức chương trình theo mô-đun

Toàn bộ chương trình được tổ chức theo các lớp chức năng riêng gồm `models`, `services`, `controllers`, `ui` và `utils`. Cách phân tách này giúp mã nguồn rõ ràng, dễ bảo trì và thuận tiện khi mở rộng thêm chức năng.

- `models`: khai báo các kiểu dữ liệu chính.
- `services`: xử lý nghiệp vụ và thao tác dữ liệu.
- `controllers`: điều phối luồng xử lý giữa giao diện và nghiệp vụ.
- `ui`: hiển thị thông tin và tương tác với người dùng.
- `utils`: chứa các hàm hỗ trợ dùng chung.

# III. TỔ CHỨC CẤU TRÚC DỮ LIỆU VÀ THUẬT TOÁN

## 1. Phát biểu bài toán

Hệ thống được xây dựng để hỗ trợ quản lý quán cơm tấm trên môi trường console. Chương trình cần cho phép người dùng thực hiện các thao tác đặt món, quản lý giỏ hàng, nhập thông tin khách hàng, thanh toán, xem lịch sử và thống kê doanh thu một cách thuận tiện.

### 1.1. Đầu vào (Input)

- Dữ liệu món ăn được đọc từ file `app/database/menu.txt` và nạp vào chương trình.
- Dữ liệu khách hàng được đọc từ file `app/database/khachhang.txt`.
- Dữ liệu lịch sử hóa đơn được đọc từ file `app/database/history.txt` và các file lịch sử theo khách hàng.
- Người dùng nhập các thông tin cần thiết qua bàn phím như mã món, số lượng, số điện thoại khách hàng, lựa chọn thanh toán và yêu cầu xem báo cáo.

### 1.2. Đầu ra (Output)

- Hiển thị menu món ăn, thông tin giỏ hàng và các thông báo xử lý trong quá trình đặt món.
- In hóa đơn thanh toán, cập nhật lịch sử đơn hàng và lưu dữ liệu vào file.
- Hiển thị thông tin khách hàng, lịch sử mua hàng và báo cáo doanh thu khi người dùng yêu cầu.
- Cập nhật kết quả xử lý sau từng chức năng như thêm món, xóa món, thanh toán và thống kê.

## 2. Cấu trúc dữ liệu

Trong chương trình, dữ liệu được tổ chức thành các kiểu cấu trúc riêng trong file `app/models/models.h`. Cách thiết kế này giúp tách biệt rõ dữ liệu món ăn, khách hàng, giỏ hàng, hóa đơn và các cấu trúc phục vụ tra cứu, từ đó thuận tiện cho việc xử lý ở các lớp `services`, `controllers` và `ui`.

### 2.1. Các hằng số và giới hạn dữ liệu

- `MAX_DON_HANG`: số lượng hóa đơn tối đa được lưu trong một số xử lý nội bộ.
- `MAX_GIO_HANG`: số món tối đa trong giỏ hàng.
- `TONG_SO_MON`: tổng số món trong menu.
- `BTREE_ORDER`: bậc của cây B-Tree dùng cho khách hàng.
- `HASH_TABLE_SIZE`: kích thước bảng băm tra cứu menu.

### 2.2. `MenuItem`

`MenuItem` dùng để lưu thông tin một món ăn trong menu.

- `id`: mã món ăn.
- `name`: tên món.
- `price`: giá của món.
- `hasOptions`: đánh dấu món có lựa chọn phụ hay không.

Kiểu này là dữ liệu gốc để nạp menu từ file và phục vụ chức năng tra cứu món.

### 2.3. `OrderItem`

`OrderItem` biểu diễn một món đã được chọn vào giỏ hàng.

- `id`: mã món đã chọn.
- `name`: tên món đã đặt.
- `price`: đơn giá của món.
- `quantity`: số lượng đặt.
- `option`: lựa chọn phụ của món nếu có.
- `note`: ghi chú thêm cho món.
- `totalPrice`: thành tiền của món trong giỏ.

Đây là đơn vị dữ liệu được dùng khi cộng tiền, chỉnh sửa số lượng và in hóa đơn.

### 2.4. `Customer`

`Customer` lưu thông tin khách hàng và dữ liệu tích lũy để phục vụ chăm sóc khách hàng.

- `id`: mã khách hàng.
- `phone`: số điện thoại.
- `name`: họ tên khách hàng.
- `address`: địa chỉ khách hàng.
- `totalSpent`: tổng tiền đã chi tiêu.
- `rank`: hạng thành viên hiện tại.

### 2.5. `CartNode` và `Cart`

Giỏ hàng được cài đặt bằng danh sách liên kết đôi để có thể thêm, xóa và cập nhật món linh hoạt.

a) `CartNode`

- `item`: lưu một món trong giỏ hàng dưới dạng `OrderItem`.
- `prev`: con trỏ tới nút đứng trước.
- `next`: con trỏ tới nút đứng sau.

`CartNode` giúp chương trình lưu từng món trong giỏ theo dạng danh sách có liên kết hai chiều.

b) `Cart`

- `head`: con trỏ tới nút đầu tiên.
- `tail`: con trỏ tới nút cuối cùng.
- `itemCount`: số lượng phần tử hiện có trong giỏ hàng.

`Cart` là cấu trúc quản lý toàn bộ danh sách món đã chọn, hỗ trợ duyệt và thao tác với giỏ hàng thuận tiện.

### 2.6. `Bill`

`Bill` lưu toàn bộ thông tin của một lần thanh toán.

- `id`: mã hóa đơn.
- `customer`: thông tin khách hàng.
- `cart`: giỏ hàng tại thời điểm thanh toán.
- `total`: tổng tiền trước giảm giá.
- `discount`: số tiền được giảm.
- `finalPrice`: số tiền phải thanh toán.
- `dateTime`: thời gian lập hóa đơn.

### 2.7. `CustomerRevenue`

`CustomerRevenue` được dùng trong phần thống kê doanh thu theo khách hàng.

- `customerId`: mã khách hàng.
- `billCount`: số lượng hóa đơn của khách hàng.
- `revenue`: tổng doanh thu khách hàng đã tạo ra.

### 2.8. `HistoryNode`

`HistoryNode` dùng để lưu lịch sử hóa đơn theo danh sách liên kết đơn.

- `customerId`: mã khách hàng.
- `bill`: dữ liệu của hóa đơn.
- `next`: con trỏ sang hóa đơn tiếp theo.

### 2.9. `BTreeNode`

`BTreeNode` là nút của cây B-Tree, được dùng để tra cứu khách hàng theo số điện thoại.

- `customers`: mảng con trỏ chứa các khách hàng.
- `children`: mảng con trỏ đến các nút con.
- `numKeys`: số khóa hiện có trong nút.
- `isLeaf`: đánh dấu nút lá hay không.

### 2.10. `HashNode` và `HashTable`

Bảng băm được dùng để tra cứu menu theo mã món với tốc độ nhanh hơn so với duyệt tuyến tính.

- `HashNode` chứa khóa, một `MenuItem` và con trỏ `next`.
- `HashTable` gồm mảng `table` và kích thước `size`.

### 2.11. Các biến dữ liệu toàn cục

Các biến và mảng chính được khai báo trong `models.h` để chia sẻ giữa các module của chương trình.

- `menu[TONG_SO_MON]`: lưu danh sách món ăn đọc từ file.
- `danhSachKH[1000]`: lưu danh sách khách hàng trong hệ thống.
- `soLuongKH`: số lượng khách hàng hiện có.
- `btreeRoot`: gốc của cây B-Tree khách hàng.
- `historyHead`: trỏ đến danh sách lịch sử hóa đơn.
- `menuHashTable`: bảng băm phục vụ tra cứu món.
- `cart`: giỏ hàng hiện tại của chương trình.

Các dữ liệu này là nền tảng cho toàn bộ luồng xử lý đặt món, thanh toán, tra cứu và thống kê.

## 3. Thuật toán

Phần này mô tả các thuật toán thực tế đã được cài đặt trong mã nguồn của dự án. Mỗi mục chỉ tập trung vào cách hoạt động của thuật toán và ví dụ code minh họa.

### 3.1. B-Tree cho tra cứu khách hàng

Cây B-Tree được dùng để lưu và tra cứu thông tin khách hàng theo khóa là số điện thoại. Khi tìm kiếm, chương trình so sánh giá trị khóa với các nút trong B-Tree và đi xuống con phù hợp đến khi tìm thấy hoặc đến nút lá. Khi chèn, nếu một nút đầy, nó được chia (split) để giữ cấu trúc cân bằng.

**Thuật toán tìm kiếm B-Tree:**
+ B1: Bắt đầu từ nút gốc.
+ B2: So sánh khóa tìm kiếm với các khóa trong nút hiện tại.
+ B3: Nếu khóa trùng khớp, trả về khách hàng tương ứng.
+ B4: Nếu khóa nhỏ hơn một khóa trong nút, đi xuống con bên trái.
+ B5: Nếu khóa lớn hơn tất cả các khóa, đi xuống con bên phải.
+ B6: Nếu đạt nút lá mà không tìm thấy, trả về NULL.

**Sơ đồ cấu trúc B-Tree:**
```
            [nút gốc: key1 | key2]
           /        |         \
    [con0]      [con1]      [con2]
   keys<key1  key1<keys<key2  keys>key2
```

**Ví dụ mã C:
```c
Customer* searchBTree(BTreeNode* root, const char* phone) {
    if (!root) return NULL;
    int i = 0;
    while (i < root->numKeys && strcmp(phone, root->customers[i]->phone) < 0) {
        i++;
    }
    if (i < root->numKeys && strcmp(phone, root->customers[i]->phone) == 0) {
        return root->customers[i];
    }
    return root->isLeaf ? NULL : searchBTree(root->children[i], phone);
}
```

- Độ phức tạp: O(log n) cho tìm kiếm và chèn trên cây B-Tree cân bằng.

### 3.2. Bảng băm cho tra cứu menu

Menu được lưu trong bảng băm sử dụng phương pháp open chaining. Mỗi món ăn có một khóa ID, được băm để xác định bucket. Nếu nhiều mục cùng bucket, chúng được nối tiếp theo danh sách xâu chuỗi.

**Thuật toán tra cứu bảng băm:**
+ B1: Tính hàm băm của ID: `hash = ID % table_size`.
+ B2: Lấy bucket tại vị trí `hash`.
+ B3: Duyệt danh sách xâu chuỗi trong bucket.
+ B4: So sánh ID của từng node với ID cần tìm.
+ B5: Nếu trùng khớp, trả về `MenuItem` tương ứng. Nếu đến cuối danh sách, trả về NULL.

**Sơ đồ bảng băm với chaining:**
```
Bảng băm (kích thước n):
[0] -> HashNode(ID1) -> HashNode(ID2) -> NULL
[1] -> HashNode(ID3) -> NULL
[2] -> NULL
...
[n-1] -> HashNode(IDk) -> NULL
```

**Ví dụ mã C:
```c
static int hash(int key, int size) {
    return key % size;
}

MenuItem *hashTableSearch(HashTable *ht, int key) {
    if (!ht) return NULL;
    int index = hash(key, ht->size);
    HashNode *node = ht->table[index];
    while (node) {
        if (node->key == key) return &node->item;
        node = node->next;
    }
    return NULL;
}
```

- Độ phức tạp: trung bình O(1), trường hợp xấu O(n) nếu nhiều va chạm.

### 3.3. Giỏ hàng (Doubly linked list)

Giỏ hàng được cài đặt bằng danh sách liên kết đôi. Mỗi mục trong giỏ là một node chứa thông tin sản phẩm và con trỏ đến node trước/sau.

**Thuật toán thêm món vào giỏ:**
+ B1: Tạo node mới với thông tin OrderItem.
+ B2: Đặt con trỏ `prev` của node mới = `tail` (nút cuối hiện tại).
+ B3: Đặt con trỏ `next` của node mới = NULL.
+ B4: Nếu giỏ không rỗng, cập nhật `next` của nút `tail` cũ để trỏ tới node mới.
+ B5: Cập nhật `tail` của giỏ = node mới.
+ B6: Tăng `itemCount`.

**Sơ đồ danh sách liên kết đôi:**
```
Cart:
 head -> [Node 1: OrderItem] <-> [Node 2: OrderItem] <-> [Node 3: OrderItem] <- tail
         (prev=NULL)            (prev/next)              (next=NULL)
```

**Ví dụ mã C:
```c
void cartAddItem(Cart* cart, OrderItem item) {
    CartNode* node = malloc(sizeof(CartNode));
    node->item = item;
    node->prev = cart->tail;
    node->next = NULL;
    if (cart->tail) cart->tail->next = node;
    else cart->head = node;
    cart->tail = node;
    cart->itemCount++;
}
```

- Độ phức tạp: thêm ở cuối O(1), tìm node theo chỉ số O(n).

### 3.4. Xử lý file và nạp dữ liệu (parsing)

Dữ liệu menu, khách hàng và lịch sử được đọc từ file bằng `fgets`, sau đó tách các trường bằng `strtok` hoặc `sscanf`.

- Mỗi dòng file được đọc thành chuỗi, loại bỏ ký tự xuống dòng.
- Chuỗi được cắt thành các trường theo dấu `|`.
- Dữ liệu trường sau đó được gán vào struct và đưa vào cấu trúc dữ liệu tương ứng.

- Ví dụ mã C:
```c
void loadMenuFromFile(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return;

    char line[200];
    int i = 0;
    menuHashTable = createHashTable();

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        char *token = strtok(line, "|");
        if (!token) continue;
        menu[i].id = atoi(token);
        token = strtok(NULL, "|");
        if (token) strcpy(menu[i].name, token);
        token = strtok(NULL, "|");
        if (token) menu[i].price = atof(token);
        token = strtok(NULL, "|");
        if (token) menu[i].hasOptions = atoi(token);
        hashTableInsert(menuHashTable, menu[i].id, menu[i]);
        i++;
        if (i >= TONG_SO_MON) break;
    }
    fclose(file);
}
```

- Độ phức tạp: O(n) theo số dòng file.

### 3.5. Lưu/nạp lịch sử và phân tích lịch sử

Lịch sử hóa đơn được lưu vào file tổng `app/database/history.txt` và file riêng theo từng khách `app/database/history_khach_<id>.txt`. Khi nạp lại, chương trình đọc từng dòng định dạng `BILL|...`, sau đó tiếp tục đọc các dòng `ITEM|...` và cuối cùng là `END` để tái tạo hóa đơn và danh sách món.

- Ghi một hóa đơn: nối một dòng `BILL` tóm tắt và lần lượt các dòng `ITEM` chi tiết vào file.
- Nạp lịch sử: đọc từng dòng, phân tách trường, tạo `Bill` và `OrderItem`, rồi thêm vào danh sách liên kết `HistoryNode`.
- Truy vấn lịch sử: duyệt danh sách liên kết hoặc tìm hóa đơn theo ID.

- Ví dụ mã C:
```c
void saveBillToGlobalFile(Bill* bill, int customerId) {
    FILE* file = fopen("app/database/history.txt", "a");
    if (!file) return;
    fprintf(file, "BILL|%d|%d|%s|%s|%s|%.3f|%.3f|%.3f|%d|%s|%.3f\n",
            bill->id,
            customerId,
            bill->customer.name,
            bill->customer.phone,
            bill->dateTime,
            bill->total,
            bill->discount,
            bill->finalPrice,
            bill->cart.itemCount,
            bill->customer.rank,
            bill->customer.totalSpent);
    CartNode *node = bill->cart.head;
    while (node) {
        fprintf(file, "ITEM|%d|%s|%.3f|%d|%d|%.3f|%s\n",
                node->item.id,
                node->item.name,
                node->item.price,
                node->item.quantity,
                node->item.option,
                node->item.totalPrice,
                node->item.note);
        node = node->next;
    }
    fprintf(file, "END\n");
    fclose(file);
}
```

- Độ phức tạp: O(n) với n là số hóa đơn, O(1) cho ghi một hóa đơn.

### 3.6. Thanh toán, subtotal và mã hóa đơn ngẫu nhiên

Khi thanh toán, chương trình duyệt giỏ hàng để tính subtotal, áp dụng giảm giá theo hạng khách và sinh ID hóa đơn số. Nếu khách hàng đã đăng ký, chương trình còn cập nhật `totalSpent` và có thể nâng hạng thành viên.

- Tổng phụ: cộng `price * quantity` của từng `OrderItem` trong giỏ.
- Giảm giá: xác định phần trăm theo `rank` của khách hàng (`Bronze`, `Silver`, `Gold`, `Diamond`).
- Sinh ID hóa đơn: tạo số ngẫu nhiên 6 chữ số.
- Cập nhật khách hàng: nếu là khách đăng ký, lưu lại `totalSpent` và `rank` vào file `khachhang.txt`.

- Ví dụ mã C:
```c
double calculateSubtotal(Bill* bill) {
    double total = 0;
    CartNode *node = bill->cart.head;
    while (node) {
        total += node->item.price * node->item.quantity;
        node = node->next;
    }
    return total;
}

double getDiscountPercent(char* rank) {
    if (strcmp(rank, RANK_DIAMOND) == 0) return 0.30;
    if (strcmp(rank, RANK_GOLD) == 0) return 0.20;
    if (strcmp(rank, RANK_SILVER) == 0) return 0.10;
    return 0.0;
}
```

- Độ phức tạp: O(n) với n là số mục trong giỏ hàng.

### 3.7. Tính toán báo cáo doanh thu

Báo cáo doanh thu được tổng hợp bằng cách duyệt danh sách hóa đơn trong lịch sử và cộng dồn các giá trị.

- Tổng doanh thu: cộng tổng giá của mỗi hóa đơn.
- Đếm hóa đơn: tăng bộ đếm cho mỗi hóa đơn.
- Đếm số món bán ra: cộng số lượng món từ mỗi hóa đơn.
- Phân tích theo khách: gom doanh thu và số hóa đơn theo khách hàng.

- Ví dụ mã C:
```c
int calculateTotalRevenue(HistoryNode* head) {
    int total = 0;
    HistoryNode* node = head;
    while (node) {
        total += node->bill.total;
        node = node->next;
    }
    return total;
}
```

- Độ phức tạp: O(n) với n là số hóa đơn trong lịch sử.

---

# IV. CHƯƠNG TRÌNH VÀ KẾT QUẢ

## 1. Tổ chức chương trình

### 1.2. Các thư viện và hằng số được sử dụng trong chương trình

Đoạn mã thực tế trong chương trình thể hiện các thư viện và hằng số chính được sử dụng như sau:

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "app/models/models.h"

#include "app/utils/helper.h"
#include "app/utils/validator.h"

#include "app/ui/menu_ui.h"
#include "app/ui/cart_ui.h"
#include "app/ui/welcome_ui.h"

#include "app/controllers/order_controller.h"
#include "app/controllers/customer_controller.h"
#include "app/controllers/checkout_controller.h"
#include "app/controllers/history_controller.h"

#include "app/services/file_service.h"
#include "app/services/history_service.h"
#include "app/services/report_service.h"
#include "app/services/hash_service.h"
#include "app/services/cart_service.h"
```

Các hằng số được định nghĩa trong file `app/models/models.h` như sau:

```c
#define MAX_DON_HANG 100
#define MAX_GIO_HANG 30
#define TONG_SO_MON 25

#define RANK_BRONZE "Bronze"
#define RANK_SILVER "Silver"
#define RANK_GOLD "Gold"
#define RANK_DIAMOND "Diamond"

#define BTREE_ORDER 4
#define HASH_TABLE_SIZE 101
```

### 1.3. Các nguyên mẫu hàm được xây dựng trong chương trình

| Nguyên mẫu hàm | Chức năng |
| --- | --- |
| `void loadMenuFromFile(const char* filename)` | Đọc file menu và nạp danh sách món ăn vào bộ nhớ, đồng thời khởi tạo bảng băm menu. |
| `void loadCustomersFromFile(BTreeNode** root)` | Đọc file khách hàng và chèn từng khách vào B-Tree để tra cứu nhanh. |
| `void saveAllCustomersToFile()` | Ghi lại tất cả dữ liệu khách hàng ra file sau khi cập nhật. |
| `void loadHistoryFromFile()` | Nạp lại lịch sử hóa đơn từ file `app/database/history.txt`. |
| `void saveBillToHistoryFile(int customerId, Bill* bill)` | Lưu hóa đơn chi tiết vào file lịch sử riêng của khách. |
| `void saveBillToGlobalFile(Bill* bill, int customerId)` | Lưu hóa đơn tóm tắt vào file lịch sử chung của quán. |
| `HashTable *createHashTable()` | Khởi tạo bảng băm dùng để tra cứu menu. |
| `void freeHashTable(HashTable *ht)` | Giải phóng bộ nhớ bảng băm khi chương trình kết thúc. |
| `void hashTableInsert(HashTable *ht, int key, MenuItem item)` | Thêm một món ăn vào bảng băm theo mã món. |
| `MenuItem *hashTableSearch(HashTable *ht, int key)` | Tìm món ăn trong bảng băm theo mã món. |
| `void cartInit(Cart *cart)` | Khởi tạo giỏ hàng rỗng trước khi thêm món. |
| `void cartAddItem(Cart *cart, OrderItem item)` | Thêm món vào giỏ hàng dưới dạng danh sách liên kết đôi. |
| `CartNode *cartGetNodeByIndex(Cart *cart, int index)` | Lấy nút giỏ hàng theo chỉ số để chỉnh sửa hoặc xóa. |
| `int cartRemoveItemByIndex(Cart *cart, int index)` | Xóa món trong giỏ hàng theo vị trí chỉ định. |
| `int cartUpdateQuantityByIndex(Cart *cart, int index, int newQuantity)` | Cập nhật số lượng món trong giỏ hàng. |
| `BTreeNode* createBTreeNode(int isLeaf)` | Tạo một nút B-Tree mới. |
| `void splitChild(BTreeNode* parent, int i, BTreeNode* child)` | Chia nút con khi B-Tree bị đầy. |
| `void insertNonFull(BTreeNode* node, Customer* cust)` | Chèn khách hàng vào nút B-Tree chưa đầy. |
| `void insertBTree(BTreeNode** root, Customer* cust)` | Chèn khách hàng vào cây B-Tree, điều chỉnh gốc nếu cần. |
| `Customer* searchBTree(BTreeNode* root, char* phone)` | Tìm khách hàng theo số điện thoại trong B-Tree. |
| `void handleOrderMenu(Bill* currentBill)` | Điều phối luồng đặt món và thêm sản phẩm vào giỏ hàng. |
| `void handleCartMenu(Bill* currentBill)` | Điều phối luồng xem và chỉnh sửa giỏ hàng. |
| `void handleCustomer(Bill *currentBill)` | Nhập và quản lý thông tin khách hàng cho hóa đơn. |
| `void processCheckout(Bill* currentBill)` | Xử lý thanh toán, tính tổng, giảm giá và lưu hóa đơn. |
| `void handleViewHistory()` | Hiển thị lịch sử đơn hàng và hóa đơn đã ghi nhận. |
| `void handleGenerateReport()` | Tạo và xuất báo cáo doanh thu. |
| `double calculateTotalRevenue()` | Tính tổng doanh thu từ các hóa đơn đã lưu. |
| `int calculateTotalBills()` | Tính tổng số hóa đơn đã ghi nhận. |
| `int calculateTotalItems()` | Tính tổng số món đã bán. |
| `void analyzeByCustomer(CustomerRevenue* customerStats, int* customerCount)` | Phân tích doanh thu theo khách hàng và tổng hợp số liệu. |
| `void handleViewHistory()` | Điều phối luồng xem lịch sử đơn hàng, bao gồm tìm theo số điện thoại và tìm theo ID hóa đơn. |
| `void handleGenerateReport()` | Điều phối luồng tổng hợp và xuất báo cáo doanh thu. |

## 2. Nhận xét đánh giá

### 2.3. Nhận xét đánh giá

- Về mặt nghiệp vụ: hệ thống đã triển khai đầy đủ luồng đặt món, quản lý giỏ hàng, nhập thông tin khách hàng, thanh toán và lưu lịch sử. Các chức năng chính được tách thành module rõ ràng, giúp hạn chế xử lý điều kiện trực tiếp trong giao diện và dễ kiểm tra hơn.
- Về mặt thuật toán: chương trình sử dụng bảng băm để tìm nhanh món ăn theo mã (`hash_service.c`) và cây B-Tree để tra cứu khách hàng theo số điện thoại (`btree_service.c`). Cấu trúc giỏ hàng sử dụng danh sách liên kết đôi (`cart_service.c`) giúp thêm/xóa món tiện lợi và hiệu quả.
- Về mặt dữ liệu: dữ liệu được lưu trong file theo định dạng `|` và nạp lại khi khởi động, phù hợp với mức độ dự án. Tuy nhiên nếu mở rộng thành hệ thống thực tế, cần chuyển sang quản trị cơ sở dữ liệu để quản lý giao dịch, khóa dữ liệu và truy vấn báo cáo tốt hơn.
- Về mặt giao diện: giao diện console hiện tại phù hợp để kiểm tra nghiệp vụ và thuật toán, các menu màu sắc và thông báo rõ ràng, nhưng giao diện Web hoặc đồ họa sẽ tạo trải nghiệm gần thực tế hơn. Các thao tác cần hoàn thiện thêm ở phần hiển thị trạng thái xử lý và hướng dẫn nhập liệu.

---

# V. KẾT LUẬN VÀ HƯỚNG PHÁT TRIỂN

## 1. Kết luận

- Trong quá trình thực hiện đồ án, nhóm đã hoàn thành được một hệ thống đặt món và thanh toán cơ bản bằng ngôn ngữ C. Chương trình hiện đã hỗ trợ lưu trữ và tra cứu menu, quản lý giỏ hàng, lưu lịch sử hóa đơn, và tính toán doanh thu.
- Nhóm đã vận dụng được nhiều kiến thức đã học như cấu trúc dữ liệu (B-Tree, bảng băm, danh sách liên kết), xử lý file, và thiết kế chương trình theo mô đun. Việc triển khai chức năng thanh toán và lưu lịch sử giúp thể hiện rõ quá trình xử lý dữ liệu thực tế.
- Mặc dù đồ án còn một số hạn chế về giao diện và tính năng mở rộng, nhóm đã cố gắng hoàn thiện các chức năng chính và đảm bảo chương trình hoạt động ổn định với dữ liệu mẫu.

## 2. Hướng phát triển

- Phát triển giao diện trực quan hơn: có thể xây dựng phần hiển thị menu và giỏ hàng thân thiện hơn, hoặc chuyển sang giao diện đồ họa đơn giản nếu dùng ngôn ngữ khác trong tương lai.
- Mở rộng các phương thức thanh toán: bổ sung thẻ ngân hàng, ví điện tử, hoặc mã QR để phù hợp với xu hướng thanh toán không dùng tiền mặt.
- Tăng cường quản lý khách hàng: thêm chức năng ghi nhận đánh giá, đặt bàn trước, hoặc gửi thông báo khuyến mãi cho khách hàng thân thiết.
- Thống kê và phân tích dữ liệu nâng cao: triển khai báo cáo doanh thu theo ngày/tuần/tháng, danh sách món bán chạy, và so sánh hiệu suất bán hàng.
- Tối ưu hiệu suất và bảo mật: cải tiến xử lý dữ liệu lớn, kiểm tra trùng lặp ID hóa đơn, và xử lý nhập liệu an toàn hơn với các hàm kiểm tra đầu vào.

---

## Tài liệu tham khảo

[1] Wikipedia. "B-tree". https://en.wikipedia.org/wiki/B-tree

[2] Wikipedia. "Hash table". https://en.wikipedia.org/wiki/Hash_table

[3] Wikipedia. "Doubly linked list". https://en.wikipedia.org/wiki/Doubly_linked_list

[4] cppreference.com. "C Input/Output <stdio.h>". https://en.cppreference.com/w/c/io

[5] cppreference.com. "C String handling <string.h>". https://en.cppreference.com/w/c/string/byte

[6] GeeksforGeeks. "Hash Table | Implementation". https://www.geeksforgeeks.org/hashing-data-structure/

[7] Open Data Structures. "B-Trees". http://opendatastructures.org/versions/edition-0.1g/ods-python/14_2_B_Trees.html

---

Nếu bạn muốn, mình có thể tiếp tục giúp viết thêm phần 4 nếu đang thiếu, hoặc tinh chỉnh nội dung cho phần 5 cho ngắn gọn hơn theo yêu cầu đồ án.
