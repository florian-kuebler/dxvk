orbit_api_src = files([
  'OrbitDXVK.cpp',
])


orbit_api_lib = static_library('orbit_api_interface', orbit_api_src,
  dependencies        : [ ],
  include_directories : [ dxvk_include_path ],
)

orbit_api_dep = declare_dependency(
  link_with           : [ orbit_api_lib ],
  include_directories : [ dxvk_include_path ],
)
