/*
 * This file is part of ATSDB.
 *
 * ATSDB is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ATSDB is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with ATSDB.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DBFILTER_H_
#define DBFILTER_H_

#include <string>
#include <vector>

#include "configurable.h"

#include "json.hpp"

class DBFilterWidget;
class DBFilterCondition;
class FilterManager;
class DBOVariable;

/**
 * @brief Dynamic database filter
 *
 * @detail Holds a number of instances of DBFilterCondition, can hold sub-filters (not used at the
 * moment), and some properties. Also at the moment only AND combined conditions can be used.
 *
 * \todo Extend to OR operations, inversion and sub-filters
 * \todo Change to DBFilter interface with common functionality, and derive SensorFilter and
 * GenericFilter
 */
class DBFilter : public Configurable
{
  public:
    /// @brief Constructor
    DBFilter(const std::string& class_id, const std::string& instance_id, Configurable* parent,
             bool is_generic = true);
    /// @brief Destructor
    virtual ~DBFilter();

    /// @brief Sets the filter active flag
    void setActive(bool active);
    /// @brief Returns the filter active flag
    bool getActive();

    /// @brief Adds a sub-filter. Not used yet.
    void addSubFilter(DBFilter* filter);
    /// @brief Returns if the DBObject of type is filtered by this filter
    virtual bool filters(const std::string& dbo_type);

    /// @brief Get if the filter configuration has changed
    bool getChanged();
    /// @brief Set if the filter configuration has changed
    void setChanged(bool changed);

    /// @brief Returns the visible flag
    bool getVisible();
    /// @brief Sets the visible flag
    void setVisible(bool visible);

    /// @brief Returns the filter name
    const std::string& getName() const { return name_; }
    /// @brief Sets the filter name
    void setName(const std::string& name);

    /// @brief Returns the generic flag
    bool isGeneric() { return is_generic_; }

    /// @brief Returns the condition string for a DBObject
    virtual std::string getConditionString(const std::string& dbo_name, bool& first,
                                           std::vector<DBOVariable*>& filtered_variables);
    /// @brief Returns if only sub-filters and no own conditions exist
    bool onlyHasSubFilter() { return conditions_.size() > 0; }

    /// @brief Returns if the filter uses the AND operation. Not used yet.
    bool getAnd() { return op_and_; }
    /// @brief Sets the filter AND operation flag. Not used yet.
    void setAnd(bool op_and);

    /// @brief Inverts the filter. Not used yet.
    void invert();

    /// @brief Resets the filter (sub-filters and conditions) to their inital values.
    virtual void reset();

    virtual void generateSubConfigurable(const std::string& class_id,
                                         const std::string& instance_id);

    /// @brief Returns the conditions container
    std::vector<DBFilterCondition*>& getConditions() { return conditions_; }
    /// @brief Returns the number of conditions
    unsigned int getNumConditions() { return conditions_.size(); }
    /// @brief Removes a specific condition
    void deleteCondition(DBFilterCondition* condition);

    DBFilterWidget* widget();

    bool disabled() const { return disabled_; }

    virtual void saveViewPointConditions (nlohmann::json& filters);
    virtual void loadViewPointConditions (nlohmann::json& filters);

  protected:
    // FilterManager &filter_manager_;
    /// Active flag, if false, no conditions are used.
    bool active_;
    /// AND operation flag. Not used.
    bool op_and_;
    /// Widget expanded flag
    bool visible_;
    bool disabled_{false};  // if a conditions is unusable
    /// Filter name
    std::string name_;

    /// Anything was changed flag.
    bool changed_;
    /// Conditions container.
    std::vector<DBFilterCondition*> conditions_;
    /// Sub-filters container. Not used yet.
    std::vector<DBFilter*> sub_filters_;

    /// Widget with configuration elements.
    DBFilterWidget* widget_{nullptr};

    /// Generic flag. Only generic (generated by configuration) can be managed (in the
    /// DBFilterWidget).
    bool is_generic_;

    virtual void checkSubConfigurables();
};

#endif /* DBFILTER_H_ */
