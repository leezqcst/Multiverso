#include "multiverso/updater/updater.h"

#include "multiverso/updater/adagrad_updater.h"
#include "multiverso/updater/smooth_gradient_updater.h"
#include "multiverso/util/configure.h"

namespace multiverso {

MV_DEFINE_string(updater_type, "default", "multiverso server updater type");

template <typename T>
void Updater<T>::Update(size_t num_element, T* data, T* delta,
                        UpdateOption* option = nullptr) {
  // parallelism with openmp
  #pragma omp parallel for schedule(static) num_threads(4)
  for (int i = 0; i < num_element; ++i) data[i] += delta[i];
}

template <typename T>
Updater<T>* Updater<T>::GetUpdater() {
  std::string type = MV_CONFIG_updater_type;
  if (type == "adagrad") return new AdagradUpdater<T>();
  if (type == "smooth_gradient") return new SmoothGradientUpdater<T>();
  // Default: simple updater
  return new Updater<T>();
}

// Temporally without int type since it will cause some warning
// TODO(feiga): add int
#define MV_INSTANTIATE_CLASS_WITH_BASE_TYPE(classname) \
  template class classname<float>;                     \
  template class classname<double>;

MV_INSTANTIATE_CLASS_WITH_BASE_TYPE(Updater);

}