define_property (TARGET
  PROPERTY GIT_DESCRIBE
  BRIEF_DOCS "Advanced version info for developers"
  FULL_DOCS "String with information that is important for developers during
  development process. This information includes git commit hash, durty status
  of repo, distance from the last tag.")

define_property (TARGET
  PROPERTY GIT_UNTRACKED_FILES
  BRIEF_DOCS "Information about presence of untracked files"
  FULL_DOCS "Used in helper functions generation to add .with-untracked suffix
  to version string. Suffix is only added if there are some untracked not
  ignored files in repository.")


function (target_version_information
	TARGET_NAME i_target_name
    EXPORT_HEADER i_export_header
    EXPORT_MACRO i_export_macro
    VERSIONED_ENTITY i_versioned_entity)
  find_file (
	headerFileTemplate
	"ProjectVersioning/version.h.in"
	PATHS ${CMAKE_MODULE_PATH})

  find_file (
	sourceFileTemplate
	"ProjectVersioning/version.c.in"
	PATHS ${CMAKE_MODULE_PATH})

  # Общие функции, выполняемые для версионирования проекта.

  # Информация о коммите.
  #
  # Необходима для идентицикации версий на этапе разработки между релизами. В
  # документации для внешнего заказчика может обозначаться, как служебная
  # информация о релизе.
  exec_program (
	"git"
	${CMAKE_SOURCE_DIR}
	ARGS "describe --always --dirty --long --tags"
	OUTPUT_VARIABLE gitDescribe)

  # Информация о незафиксированных изменениях.
  #
  # В случае, если в репозитории имеются незафиксированные изменения, к строке
  # версии будет добавлено слово dirty. Это необходимо для того, чтобы была
  # возможность определить факт того, что собранная версия может включать в себя
  # какие-то изменения, которые не были зафиксированы, а значит не годится для
  # использования где-либо, кроме тестового стенда.
  #
  # При формировании значения учитываются файлы, не находящиеся под контролем
  # версий. В случае, если такие файлы присутствуют и не используются при
  # сборке, необходимо настроить их глобальное или локальное игнорирование.
  exec_program (
	"git"
	${CMAKE_SOURCE_DIR}
	ARGS "ls-files --others --exclude-standard"
	OUTPUT_VARIABLE gitUntracked)

  if (gitUntracked)
	set (gitUntracked ".with-untracked")
  endif (gitUntracked)

  # Информация о версии ПО.
  #
  # Формируется в формате мажор.минор.патч в соответствии с Semantic
  # Versioning. Подробную информацию о том, как происходит присваивание версий
  # можно найти в официальном описании техники версионирования.
  configure_file (
	"${headerFileTemplate}"
	"${CMAKE_CURRENT_BINARY_DIR}/${i_versioned_entity}_version.h")

  configure_file(
	"${sourceFileTemplate}"
	"${CMAKE_BINARY_DIR}/${i_versioned_entity}_version.c")

  target_sources ("${i_target_name}"
    PRIVATE
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/${i_versioned_entity}_version.h>
    $<INSTALL_INTERFACE:include/${i_include_prefix}/${i_versioned_entity}_version.h>
	PRIVATE
	"${CMAKE_BINARY_DIR}/${i_versioned_entity}_version.c")

  set_target_properties (${i_target_name}
	PROPERTIES
	GIT_DESCRIBE "${gitDescribe}"
	GIT_UNTRACKED_FILES "${gitUntracked}")

  unset (headerFileTemplate PARENT_SCOPE)
  unset (sourceFileTemplate PARENT_SCOPE)
endfunction (target_version_information)
