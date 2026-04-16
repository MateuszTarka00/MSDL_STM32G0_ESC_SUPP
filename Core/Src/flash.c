#include "flash.h"
#include <string.h>

#define FLASH_PAGE              14
#define FLASH_BASE_ADDR         0x08000000U
#define FLASH_MAGIC             0xDEADBEEFUL
#define FLASH_DATA_SIZE (sizeof(Flash_parametersToSave) - sizeof(uint32_t))

Flash_parametersToSave flash_parametersToSave = {};

// ===================== LOW LEVEL =====================

static uint32_t Flash_GetPageAddress(uint32_t pageIndex)
{
    return FLASH_BASE_ADDR + (pageIndex * FLASH_PAGE_SIZE);
}

static void Flash_WaitForLastOperation(void)
{
    while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY)) {}
}

static void Flash_ClearAllErrors(void)
{
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP |
                          FLASH_FLAG_WRPERR |
                          FLASH_FLAG_PGAERR |
                          FLASH_FLAG_SIZERR |
                          FLASH_FLAG_OPTVERR);
}

// ===================== ERASE =====================

static bool Flash_ErasePage(uint32_t pageIndex)
{
    HAL_FLASH_Unlock();

    Flash_WaitForLastOperation();
    Flash_ClearAllErrors();

    FLASH_EraseInitTypeDef eraseInit = {0};
    uint32_t pageError = 0;

    eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseInit.Page = pageIndex;
    eraseInit.NbPages = 1;

    if (HAL_FLASHEx_Erase(&eraseInit, &pageError) != HAL_OK)
    {
        HAL_FLASH_Lock();
        return FALSE;
    }

    Flash_WaitForLastOperation();
    HAL_FLASH_Lock();

    return TRUE;
}

// ===================== WRITE =====================
static bool Flash_WriteStruct(uint32_t pageIndex, const Flash_parametersToSave *data)
{
    HAL_FLASH_Unlock();

    Flash_WaitForLastOperation();
    Flash_ClearAllErrors();

    uint32_t address = Flash_GetPageAddress(pageIndex);
    const uint8_t *src = (const uint8_t *)data;

    for (uint32_t i = 0; i < sizeof(Flash_parametersToSave); i += 8)
    {
        uint64_t dword = 0xFFFFFFFFFFFFFFFFULL;

        uint32_t remaining = sizeof(Flash_parametersToSave) - i;
        memcpy(&dword, src + i, (remaining >= 8) ? 8 : remaining);

        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address + i, dword) != HAL_OK)
        {
            HAL_FLASH_Lock();
            return FALSE;
        }

        Flash_WaitForLastOperation();
    }

    // Ensure data is really committed
    __DSB();
    __ISB();

    HAL_FLASH_Lock();

	return TRUE;
}

// ===================== VERIFY =====================

static bool Flash_Verify(uint32_t pageIndex, const Flash_parametersToSave *data)
{
    const Flash_parametersToSave *flash_ptr =
        (const Flash_parametersToSave *)Flash_GetPageAddress(pageIndex);

    return (memcmp(flash_ptr, data, sizeof(Flash_parametersToSave)) == 0);
}

// ===================== CRC =====================

uint32_t crc32_compute(const uint8_t *data, uint32_t length)
{
    uint32_t crc = 0xFFFFFFFF;

    for (uint32_t i = 0; i < length; i++)
    {
        crc ^= data[i];

        for (uint8_t j = 0; j < 8; j++)
        {
            crc = (crc & 1) ? (crc >> 1) ^ 0xEDB88320UL : (crc >> 1);
        }
    }

    return ~crc;
}

// ===================== PUBLIC API =====================

bool flash_parametersSave(void)
{
    Flash_parametersToSave temp;

    memcpy(&temp, &flash_parametersToSave, sizeof(temp));

    temp.magic = FLASH_MAGIC;
    temp.crc = crc32_compute((uint8_t*)&temp, FLASH_DATA_SIZE);

    __disable_irq();

    bool ok = Flash_ErasePage(FLASH_PAGE);

    if (ok)
        ok = Flash_WriteStruct(FLASH_PAGE, &temp);

    __enable_irq();

    if (!ok)
        return FALSE;

    // 🔥 CRITICAL: verify AFTER operation
    if (!Flash_Verify(FLASH_PAGE, &temp))
        return FALSE;

    return TRUE;
}

bool flash_loadParameters(void)
{
    Flash_parametersToSave *flash_ptr =
        (Flash_parametersToSave*)Flash_GetPageAddress(FLASH_PAGE);

    if (flash_ptr->magic != FLASH_MAGIC)
        return FALSE;

    uint32_t crc_calc = crc32_compute(
        (uint8_t*)flash_ptr,
        FLASH_DATA_SIZE);

    if (crc_calc != flash_ptr->crc)
        return FALSE;

    memcpy(&flash_parametersToSave, flash_ptr, sizeof(Flash_parametersToSave));

    return TRUE;
}

void flash_factoryReset(void)
{
    __disable_irq();
    Flash_ErasePage(FLASH_PAGE);
    __enable_irq();
}
