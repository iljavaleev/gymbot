add_library(bot
src/database/DB.cpp 
src/handlers/Handlers.cpp 
src/keyboards/Keyboards.cpp 
src/utils/Utils.cpp
)

target_include_directories(bot PUBLIC include)
