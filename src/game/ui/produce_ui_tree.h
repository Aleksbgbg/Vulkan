#ifndef VULKAN_SRC_GAME_UI_PRODUCE_UI_TREE_H_
#define VULKAN_SRC_GAME_UI_PRODUCE_UI_TREE_H_

#include <optional>

#include "ParseTree.h"
#include "UiTree.h"
#include "ViewModel.h"

std::optional<UiTree> ProduceUiTree(
    const std::optional<ui::ParseTree>& parseTree, const ViewModel& viewModel);

#endif  // VULKAN_SRC_GAME_UI_PRODUCE_UI_TREE_H_
