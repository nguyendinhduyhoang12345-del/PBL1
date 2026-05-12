#ifndef HISTORY_UI_H
#define HISTORY_UI_H

#include "../services/history_service.h"
<<<<<<< HEAD
#include "../models/models.h"
=======
>>>>>>> 18329fa10e66c70c2d31276e6e4db4bd7d213247

// In toàn bộ lịch sử (tóm tắt)
void printAllHistory();

// In lịch sử hóa đơn chi tiết của 1 khách hàng (từ file khách hàng)
void printCustomerHistory(HistoryNode* historyHead);

// In chi tiết hóa đơn theo ID bill
void printBillDetail(HistoryNode* billNode);

#endif