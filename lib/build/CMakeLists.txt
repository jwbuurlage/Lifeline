CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

SET(
	LIB_NAME
	"lifeline"
)

SET(
	PROJECT_SOURCES
	"../src/LFeatures.cpp"
  "../src/LImageProcessor.cpp"
	"../src/LRecognizer.cpp"
	"../src/LDatabase.c"
)

SET(
	PROJECT_INCLUDES
	"../include"
)

#SET(
#	PROJECT_LIBRARIES
#)

#----------- DO NOT EDIT BELOW --------------

INCLUDE_DIRECTORIES( ${PROJECT_INCLUDES} )
ADD_LIBRARY( ${LIB_NAME} STATIC ${PROJECT_SOURCES} )
#TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${PROJECT_LIBRARIES} )
