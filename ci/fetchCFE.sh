# intended to be run from root of repo and will end at root of repo
pwd
git clone https://github.com/nasa/cFE ./cFE
cd cFE
# get OSAL (needed for common_types.h)
git submodule init osal/
git submodule init apps/cfs_lib/
git submodule update
sed -i.bak 's/\r$//' ./setvars.sh
# fix UT assert errors
sed -i '481s/.*/int32 CFE_ES_PoolCreate(CFE_ES_MemHandle_t *HandlePtr, uint8 *MemPtr, uint32 Size)/' tools/ut_assert/src/ut_cfe_es_stubs.c
sed -i '489s/.*/return(Ut_CFE_ES_HookTable.CFE_ES_PoolCreate((uint32*)HandlePtr, MemPtr, Size));/' tools/ut_assert/src/ut_cfe_es_stubs.c
sed -i '494s/.*/int32 CFE_ES_PoolCreateEx(CFE_ES_MemHandle_t *HandlePtr, uint8 *MemPtr, uint32 Size, uint32 NumBlockSizes, uint32 *BlockSizes, uint16 UseMutex)/' tools/ut_assert/src/ut_cfe_es_stubs.c
sed -i '502s/.*/return(Ut_CFE_ES_HookTable.CFE_ES_PoolCreateEx((uint32*)HandlePtr, MemPtr, Size, NumBlockSizes, BlockSizes, UseMutex));/' tools/ut_assert/src/ut_cfe_es_stubs.c
cd ../
