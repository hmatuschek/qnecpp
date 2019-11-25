set(qnecpp_SOURCES main.cc object.cc model.cc geometry.cc expression.cc ground.cc excitation.cc
    measurement.cc api.cc optimize.cc)
set(qnecpp_MOC_HEADERS)
set(qnecpp_HEADERS object.h model.h geometry.h ground.h excitation.h api.h measurement.h optimize.h)
set(qnecpp_UI_FORMS)
SET(CMAKE_AUTOUIC ON)
qt5_wrap_cpp(qnecpp_MOC_SOURCES ${qnecpp_MOC_HEADERS})
#qt5_add_resources(qnecpp_RCC_SOURCES ../shared/resources.qrc)

add_executable(qnecpp ${qnecpp_SOURCES} ${qnecpp_MOC_SOURCES} ${qnecpp_RCC_SOURCES} ${qnecpp_UI_FORMS})
target_link_libraries(qnecpp ${LIBS} necpp)
