# imgui-m: GL Runtime Boundary Plan

> 状态: merged checkpoint; display-required runtime execution remains follow-up
> 分支: `codex/gl-runtime-closure-imgui` (merged via PR #1)
> PR: https://github.com/mcpplibs/imgui-m/pull/1
> Last updated: 2026-06-03
> 目标: 保持 ImGui module wrapper 简洁,提供可诊断的最小窗口示例,并明确 GL runtime 由 mcpp/mcpp-index 层闭合。

## Scope

This repository owns the ImGui module wrapper and examples. It should prove
that consumers can write a minimal window application with module imports, but
it should not package platform OpenGL/GLX/EGL drivers.

## Current Work

- `examples/minimal_window/` demonstrates a module-only consumer path.
- GLFW error forwarding lets examples report the actual window creation
  failure instead of silently exiting.
- README and architecture docs explain that `compat.opengl` provides headers
  and registry data, not platform GL runtime libraries.

## Implementation Plan

- [x] Create this repository-level plan checkpoint.
- [x] Add a minimal module-only window example.
- [x] Add GLFW error accessors for backend diagnostics.
- [x] Add test coverage for exported diagnostic API presence.
- [x] Adjust the minimal example to capture GLFW error details before cleanup
      when a cleanup call could clear the last error.
- [x] Keep README and architecture docs aligned with the final mcpp-index
      package names.
  - If the runtime provider lands as `compat.glvnd`, use that name.
  - If the package name changes during implementation, update docs only after
    the index PR settles.
- [x] Add or update CI build-checks for `examples/minimal_window`.
  - Runtime execution should stay conditional on a visible display and OpenGL
    runtime.

## Non-Goals

- Do not bundle `libGL`, GLX/EGL, Mesa, NVIDIA, or host GPU driver files here.
- Do not hard-code host library paths in example source.
- Do not make headless CI depend on a real display.
- Do not change `compat.opengl` semantics from this repository.

## Verification

- [x] `mcpp build`
- [x] `mcpp test`
- [x] `cd examples/basic && mcpp run`
- [x] `cd examples/minimal_window && mcpp build`
- [x] `cd examples/glfw_opengl3 && mcpp build`
- [ ] On a machine with working display/runtime closure:
      `cd examples/minimal_window && mcpp run`

## PR / CI / Merge Notes

- [x] Commit this plan as the first checkpoint on the feature branch.
- [x] Keep current implementation checkpoint commits small.
- [x] Open a PR with sanitized paths and no local machine details.
- [x] Include a test plan that separates build-only CI from display-required
      runtime checks.
- [x] Wait for CI.
- [x] Squash merge after required checks pass.
  - PR #1 merged at commit `9bf008f343a2afbf6d77f62ea412b5a9850a1005`.

## Cross-Repository Dependencies

- `mcpp` must implement runtime metadata/run environment support before the
  minimal window example can be expected to run without manual environment
  setup.
- `mcpp-index` must express GLFW/OpenGL runtime requirements through package
  metadata.
- `xim-pkgindex` is not required for this repository PR unless a released mcpp
  version becomes a package-index dependency.
