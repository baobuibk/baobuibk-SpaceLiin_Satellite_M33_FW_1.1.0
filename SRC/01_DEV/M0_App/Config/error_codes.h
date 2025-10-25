#ifndef _ERROR_CODE_H_
#define _ERROR_CODE_H_

// Mã lỗi (thay cho #define) — giữ nguyên tên hằng
typedef enum
{
    // Mã lỗi chung (0x00 - 0x0F)
    ERROR_OK                = 0x00,  // Thành công
    ERROR_FAIL              = 0x01,  // Lỗi chung
    ERROR_TIMEOUT           = 0x02,  // Hết thời gian chờ
    ERROR_INVALID_PARAM     = 0x03,  // Tham số không hợp lệ
    ERROR_NOT_READY         = 0x04,  // Thiết bị chưa sẵn sàng
    ERROR_BUSY              = 0x05,  // Thiết bị đang bận
    ERROR_NOT_SUPPORTED     = 0x06,  // Chức năng không được hỗ trợ
    ERROR_OUT_OF_MEMORY     = 0x07,  // Hết bộ nhớ

    ERROR_BUFFER_FULL       = 0x08,  // Bộ đệm đầy
    ERROR_BUFFER_EMPTY      = 0x09,  // Bộ đệm rỗng

    // Mã lỗi cho ADC (0x10 - 0x1F)
    ERROR_ADC_OVERRUN       = 0x10,  // Lỗi overrun (dữ liệu bị ghi đè)
    ERROR_ADC_NOT_INIT      = 0x11,  // ADC chưa được khởi tạo
    ERROR_ADC_CHANNEL       = 0x12,  // Kênh ADC không hợp lệ
    ERROR_ADC_TIMEOUT       = 0x13,  // Timeout khi chuyển đổi ADC
    ERROR_ADC_CALIBRATION   = 0x14,  // Lỗi hiệu chuẩn ADC

    // Mã lỗi cho UART/USART (0x20 - 0x2F)
    ERROR_UART_OVERRUN      = 0x20,  // Lỗi overrun (dữ liệu bị ghi đè)
    ERROR_UART_NOT_INIT     = 0x21,  // UART chưa được khởi tạo
    ERROR_UART_FRAMING      = 0x22,  // Lỗi framing (dữ liệu không đúng định dạng)
    ERROR_UART_PARITY       = 0x23,  // Lỗi parity
    ERROR_UART_TIMEOUT      = 0x24,  // Timeout khi truyền/nhận
    ERROR_UART_NOISE        = 0x25,  // Lỗi nhiễu trên đường truyền

    // Mã lỗi cho SPI (0x30 - 0x3F)
    ERROR_SPI_NOT_INIT      = 0x30,  // SPI chưa được khởi tạo
    ERROR_SPI_TIMEOUT       = 0x31,  // Timeout khi truyền/nhận
    ERROR_SPI_MODF          = 0x32,  // Lỗi mode fault (master/slave xung đột)
    ERROR_SPI_OVERRUN       = 0x33,  // Lỗi overrun
    ERROR_SPI_BUSY          = 0x34,  // SPI đang bận

    // Mã lỗi cho I2C (0x40 - 0x4F)
    ERROR_I2C_NOT_INIT      = 0x40,  // I2C chưa được khởi tạo
    ERROR_I2C_TIMEOUT       = 0x41,  // Timeout khi truyền/nhận
    ERROR_I2C_NACK          = 0x42,  // Lỗi không nhận ACK
    ERROR_I2C_ARBITRATION   = 0x43,  // Lỗi mất arbitration
    ERROR_I2C_BUS_ERROR     = 0x44,  // Lỗi bus I2C

    // Mã lỗi cho GPIO (0x50 - 0x5F)
    ERROR_GPIO_NOT_INIT     = 0x50,  // GPIO chưa được khởi tạo
    ERROR_GPIO_INVALID_PIN  = 0x51,  // Chân GPIO không hợp lệ
    ERROR_GPIO_MODE         = 0x52,  // Chế độ GPIO không hợp lệ

    // Mã lỗi cho TIM (Timer) (0x60 - 0x6F)
    ERROR_TIM_NOT_INIT      = 0x60,  // Timer chưa được khởi tạo
    ERROR_TIM_CONFIG        = 0x61,  // Lỗi cấu hình timer
    ERROR_TIM_TIMEOUT       = 0x62,  // Timeout khi chờ sự kiện timer

    // Mã lỗi cho DMA (0x70 - 0x7F)
    ERROR_DMA_NOT_INIT      = 0x70,  // DMA chưa được khởi tạo
    ERROR_DMA_TRANSFER      = 0x71,  // Lỗi truyền DMA
    ERROR_DMA_TIMEOUT       = 0x72,  // Timeout khi truyền DMA
    ERROR_DMA_CONFIG        = 0x73   // Lỗi cấu hình DMA
} error_code_t;

enum {
	POWER_OFF = 0,
	POWER_ON
};

enum {
	LOW = 0,
	HIGH
};

#endif /* _ERROR_CODE_H_ */